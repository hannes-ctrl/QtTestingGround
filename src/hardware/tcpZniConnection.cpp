#include "tcpZniConnection.h"

#include <QDeadlineTimer>
#include <QTcpSocket>

namespace {

/// Obergrenze für zwischengespeicherte Telegramme, z. B. bei zyklischen Statusmeldungen
const int MaxPendingTelegrams = 200;

}

TcpZniConnection::TcpZniConnection(const QString &host, const quint16 port, const int timeoutMs)
    : m_host(host)
    , m_port(port)
    , m_timeoutMs(timeoutMs)
{
}

TcpZniConnection::~TcpZniConnection()
{
    close();
}

bool TcpZniConnection::open()
{
    close();

    m_socket = std::make_unique<QTcpSocket>();
    m_socket->connectToHost(m_host, m_port);
    if (!m_socket->waitForConnected(m_timeoutMs)) {
        m_errorString = QStringLiteral("Keine Verbindung zu %1:%2 (%3)")
                            .arg(m_host).arg(m_port).arg(m_socket->errorString());
        m_socket.reset();
        return false;
    }

    m_errorString.clear();
    return true;
}

void TcpZniConnection::close()
{
    m_buffer.clear();
    m_pending.clear();

    if (m_socket == nullptr) {
        return;
    }

    m_socket->disconnectFromHost();
    m_socket.reset();
}

bool TcpZniConnection::isOpen() const
{
    return m_socket != nullptr && m_socket->state() == QAbstractSocket::ConnectedState;
}

bool TcpZniConnection::send(const Telegram &request, Telegram &response)
{
    if (!isOpen()) {
        m_errorString = QStringLiteral("Verbindung ist nicht geöffnet");
        return false;
    }
    if (!request.isValid()) {
        m_errorString = QStringLiteral("Telegramm ist länger als %1 Byte").arg(Telegram::Size);
        return false;
    }

    // Ältere Telegramme mit demselben Kommando (z. B. zyklische Meldungen) sind keine Antwort auf diese Anfrage
    readAvailable();
    Telegram stale;
    while (takePending(request.command(), stale)) {
    }

    m_socket->write(request.data());
    while (m_socket->bytesToWrite() > 0) {
        if (!m_socket->waitForBytesWritten(m_timeoutMs)) {
            m_errorString = QStringLiteral("Senden fehlgeschlagen (%1)").arg(m_socket->errorString());
            return false;
        }
    }

    if (!waitFor(request.command(), response, m_timeoutMs)) {
        m_errorString = QStringLiteral("Keine Antwort innerhalb von %1 ms (%2)").arg(m_timeoutMs).arg(m_errorString);
        return false;
    }

    m_errorString.clear();
    return true;
}

bool TcpZniConnection::receive(const quint8 command, Telegram &telegram, const int timeoutMs)
{
    if (!isOpen()) {
        m_errorString = QStringLiteral("Verbindung ist nicht geöffnet");
        return false;
    }

    if (!waitFor(command, telegram, timeoutMs)) {
        m_errorString = QStringLiteral("Kein Telegramm %1 innerhalb von %2 ms (%3)")
                            .arg(command, 2, 16, QLatin1Char('0')).arg(timeoutMs).arg(m_errorString);
        return false;
    }

    m_errorString.clear();
    return true;
}

QString TcpZniConnection::errorString() const
{
    return m_errorString;
}

void TcpZniConnection::readAvailable()
{
    m_buffer.append(m_socket->readAll());

    while (m_buffer.size() >= Telegram::Size) {
        m_pending.append(Telegram(m_buffer.left(Telegram::Size)));
        m_buffer.remove(0, Telegram::Size);
    }

    if (m_pending.size() > MaxPendingTelegrams) {
        m_pending.remove(0, m_pending.size() - MaxPendingTelegrams);
    }
}

bool TcpZniConnection::takePending(const quint8 command, Telegram &telegram)
{
    for (int i = 0; i < m_pending.size(); ++i) {
        if (m_pending.at(i).command() == command) {
            telegram = m_pending.takeAt(i);
            return true;
        }
    }
    return false;
}

bool TcpZniConnection::waitFor(const quint8 command, Telegram &telegram, const int timeoutMs)
{
    const QDeadlineTimer deadline(timeoutMs);

    while (true) {
        readAvailable();
        if (takePending(command, telegram)) {
            return true;
        }

        if (deadline.hasExpired()) {
            m_errorString = QStringLiteral("Zeitüberschreitung");
            return false;
        }
        if (!m_socket->waitForReadyRead(static_cast<int>(deadline.remainingTime()))) {
            m_errorString = m_socket->state() == QAbstractSocket::ConnectedState
                                ? QStringLiteral("Zeitüberschreitung")
                                : QStringLiteral("Verbindung getrennt");
            return false;
        }
    }
}
