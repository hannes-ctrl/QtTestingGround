#include "logger.h"

Logger::Logger() = default;

Logger::~Logger() = default;

void Logger::log(const Level level, const QString &message)
{
    Q_UNUSED(level)
    Q_UNUSED(message)

    // TODO: Ausgabeziel festlegen (Datei, GUI)
}
