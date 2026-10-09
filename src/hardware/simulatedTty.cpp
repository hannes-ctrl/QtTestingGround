#include "simulatedTty.h"

SimulatedTty::SimulatedTty()
    : m_open(false)
{
}

SimulatedTty::~SimulatedTty() = default;

bool SimulatedTty::open()
{
    m_open = true;
    return m_open;
}

void SimulatedTty::close()
{
    m_open = false;
}

bool SimulatedTty::isOpen() const
{
    return m_open;
}
