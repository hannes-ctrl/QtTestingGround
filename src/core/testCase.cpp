#include "testCase.h"

TestCase::TestCase(const QString &name)
    : m_name(name)
{
}

TestCase::~TestCase() = default;

QString TestCase::name() const
{
    return m_name;
}
