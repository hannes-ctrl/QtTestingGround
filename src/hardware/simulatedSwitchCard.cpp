#include "simulatedSwitchCard.h"

SimulatedSwitchCard::SimulatedSwitchCard()
    : m_open(false)
{
}

SimulatedSwitchCard::~SimulatedSwitchCard() = default;

bool SimulatedSwitchCard::open()
{
    m_open = true;
    return m_open;
}

void SimulatedSwitchCard::close()
{
    m_open = false;
}

bool SimulatedSwitchCard::isOpen() const
{
    return m_open;
}
