#include "testContext.h"

TestContext::TestContext(ZniConnection *connection, Config *config, Logger *logger)
    : m_connection(connection)
    , m_config(config)
    , m_logger(logger)
    , m_abortRequested(false)
{
}

TestContext::~TestContext() = default;

ZniConnection *TestContext::connection() const
{
    return m_connection;
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
