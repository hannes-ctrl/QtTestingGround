#include "config.h"

Config::Config() = default;

Config::~Config() = default;

bool Config::load(const QString &filePath)
{
    Q_UNUSED(filePath)

    // TODO: Einstellungen einlesen
    return false;
}

QString Config::reportDirectory() const
{
    return m_reportDirectory;
}
