#include "tcpZniConnection.h"

#include <QTcpSocket>

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

    // Reste einer früheren Antwort verwerfen
    m_socket->readAll();

    m_socket->write(request.data());
    while (m_socket->bytesToWrite() > 0) {
        if (!m_socket->waitForBytesWritten(m_timeoutMs)) {
            m_errorString = QStringLiteral("Senden fehlgeschlagen (%1)").arg(m_socket->errorString());
            return false;
        }
    }

    // TODO: Annahme, dass die Antwort ebenfalls 40 Byte lang ist; mit der Telegrammbeschreibung abgleichen
    QByteArray buffer;
    while (buffer.size() < Telegram::Size) {
        if (m_socket->bytesAvailable() == 0 && !m_socket->waitForReadyRead(m_timeoutMs)) {
            m_errorString = QStringLiteral("Keine vollständige Antwort innerhalb von %1 ms (%2 von %3 Byte)")
                                .arg(m_timeoutMs).arg(buffer.size()).arg(Telegram::Size);
            return false;
        }
        buffer.append(m_socket->read(Telegram::Size - buffer.size()));
    }

    response = Telegram(buffer);
    m_errorString.clear();
    return true;
}

QString TcpZniConnection::errorString() const
{
    return m_errorString;
}
