#include "testContext.h"

TestContext::TestContext(ZlvBusInterface *zlvBus, TtyInterface *tty, SwitchCardInterface *switchCard,
                         Config *config, Logger *logger)
    : m_zlvBus(zlvBus)
    , m_tty(tty)
    , m_switchCard(switchCard)
    , m_config(config)
    , m_logger(logger)
    , m_abortRequested(false)
{
}

TestContext::~TestContext() = default;

ZlvBusInterface *TestContext::zlvBus() const
{
    return m_zlvBus;
}

TtyInterface *TestContext::tty() const
{
    return m_tty;
}

SwitchCardInterface *TestContext::switchCard() const
{
    return m_switchCard;
}

Config *TestContext::config() const
{
    return m_config;
}

Logger *TestContext::logger() const
{
    return m_logger;
}

QString TestContext::serialNumber() const
{
    return m_serialNumber;
}

void TestContext::setSerialNumber(const QString &serialNumber)
{
    m_serialNumber = serialNumber;
}

bool TestContext::abortRequested() const
{
    return m_abortRequested;
}

void TestContext::setAbortRequested(const bool abortRequested)
{
    m_abortRequested = abortRequested;
}
