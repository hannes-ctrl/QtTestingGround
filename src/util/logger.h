#ifndef ZNI_BG_TESTING_LOGGER_H
#define ZNI_BG_TESTING_LOGGER_H

#include <QString>

/**
 * @brief Protokolliert Abläufe und Rohdaten der Kommunikation für die Fehlersuche.
 */
class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error
    };

    Logger();
    ~Logger();

    void log(Level level, const QString &message);

private:
};

#endif //ZNI_BG_TESTING_LOGGER_H
