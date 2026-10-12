#include "config.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Config::Config()
    : m_host(QStringLiteral("10.90.78.33"))
    , m_port(2000)
    , m_timeoutMs(3000)
    , m_simulation(false)
    , m_reportDirectory(QCoreApplication::applicationDirPath() + QStringLiteral("/protokolle"))
{
}

Config::~Config() = default;

bool Config::load(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject()) {
        return false;
    }

    const QJsonObject root = document.object();
    m_host = root.value(QStringLiteral("host")).toString(m_host);
    m_port = static_cast<quint16>(root.value(QStringLiteral("port")).toInt(m_port));
    m_timeoutMs = root.value(QStringLiteral("timeoutMs")).toInt(m_timeoutMs);
    m_simulation = root.value(QStringLiteral("simulation")).toBool(m_simulation);
    m_reportDirectory = root.value(QStringLiteral("reportDirectory")).toString(m_reportDirectory);
    return true;
}

QString Config::host() const
{
    return m_host;
}

quint16 Config::port() const
{
    return m_port;
}

int Config::timeoutMs() const
{
    return m_timeoutMs;
}

bool Config::simulation() const
{
    return m_simulation;
}

QString Config::reportDirectory() const
{
    return m_reportDirectory;
}
