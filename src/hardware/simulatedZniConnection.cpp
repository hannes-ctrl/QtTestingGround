#include "simulatedZniConnection.h"

namespace {

quint16 selectiveKey(const quint8 command, const quint8 thirdByte)
{
    return static_cast<quint16>((command << 8) | thirdByte);
}

}

SimulatedZniConnection::SimulatedZniConnection()
    : m_open(false)
{
}

SimulatedZniConnection::~SimulatedZniConnection() = default;

void SimulatedZniConnection::setResponse(const quint8 command, const Telegram &response)
{
    m_responses.insert(command, response);
}

void SimulatedZniConnection::setResponse(const quint8 command, const quint8 thirdByte, const Telegram &response)
{
    m_selectiveResponses.insert(selectiveKey(command, thirdByte), response);
}

void SimulatedZniConnection::queueTelegram(const Telegram &telegram)
{
    m_queue.append(telegram);
}

bool SimulatedZniConnection::open()
{
    m_open = true;
    m_errorString.clear();
    return true;
}

void SimulatedZniConnection::close()
{
    m_open = false;
}

bool SimulatedZniConnection::isOpen() const
{
    return m_open;
}

bool SimulatedZniConnection::send(const Telegram &request, Telegram &response)
{
    if (!m_open) {
        m_errorString = QStringLiteral("Verbindung ist nicht geöffnet");
        return false;
    }
    if (!request.isValid()) {
        m_errorString = QStringLiteral("Telegramm ist länger als %1 Byte").arg(Telegram::Size);
        return false;
    }

    const quint16 key = selectiveKey(request.command(), request.byteAt(2));
    if (m_selectiveResponses.contains(key)) {
        response = m_selectiveResponses.value(key);
    } else {
        response = m_responses.value(request.command(), request);
    }
    m_errorString.clear();
    return true;
}

bool SimulatedZniConnection::receive(const quint8 command, Telegram &telegram, const int timeoutMs)
{
    Q_UNUSED(timeoutMs)

    for (int i = 0; i < m_queue.size(); ++i) {
        if (m_queue.at(i).command() == command) {
            telegram = m_queue.takeAt(i);
            m_errorString.clear();
            return true;
        }
    }

    m_errorString = QStringLiteral("Kein Telegramm %1 bereitgelegt (Simulation)")
                        .arg(command, 2, 16, QLatin1Char('0'));
    return false;
}

QString SimulatedZniConnection::errorString() const
{
    return m_errorString;
}
