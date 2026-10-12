#ifndef ZNI_BG_TESTING_CONFIG_H
#define ZNI_BG_TESTING_CONFIG_H

#include <QString>

/**
 * @brief Einstellungen der Testsoftware (Verbindung zum Sim-ZNI, Timeout, Ablageort der Protokolle).
 *
 * Ohne Konfigurationsdatei gelten die Standardwerte aus der Testspezifikation.
 */
class Config {
public:
    Config();
    ~Config();

    /**
     * @brief Lädt die Einstellungen aus einer JSON-Datei. Fehlende Einträge behalten ihren Standardwert.
     * @return true bei Erfolg, false wenn die Datei fehlt oder kein gültiges JSON enthält
     */
    bool load(const QString &filePath);

    QString host() const;
    quint16 port() const;
    int timeoutMs() const;

    /**
     * @brief true, wenn statt des Sim-ZNI die simulierte Verbindung verwendet werden soll.
     */
    bool simulation() const;

    QString reportDirectory() const;

private:
    QString m_host;
    quint16 m_port;
    int m_timeoutMs;
    bool m_simulation;
    QString m_reportDirectory;
};

#endif //ZNI_BG_TESTING_CONFIG_H
