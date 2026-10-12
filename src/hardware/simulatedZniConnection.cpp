#include "simulatedZniConnection.h"

SimulatedZniConnection::SimulatedZniConnection()
    : m_open(false)
{
}

SimulatedZniConnection::~SimulatedZniConnection() = default;

void SimulatedZniConnection::setResponse(const quint8 command, const Telegram &response)
{
    m_responses.insert(command, response);
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

    response = m_responses.value(request.command(), request);
    m_errorString.clear();
    return true;
}

QString SimulatedZniConnection::errorString() const
{
    return m_errorString;
}
