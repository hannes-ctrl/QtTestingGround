#include "simulatedZlvBus.h"

SimulatedZlvBus::SimulatedZlvBus()
    : m_open(false)
{
}

SimulatedZlvBus::~SimulatedZlvBus() = default;

bool SimulatedZlvBus::open()
{
    m_open = true;
    return m_open;
}

void SimulatedZlvBus::close()
{
    m_open = false;
}

bool SimulatedZlvBus::isOpen() const
{
    return m_open;
}
