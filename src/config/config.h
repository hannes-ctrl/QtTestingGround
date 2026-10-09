#ifndef ZNI_BG_TESTING_CONFIG_H
#define ZNI_BG_TESTING_CONFIG_H

#include <QString>

/**
 * @brief Einstellungen der Testsoftware (Schnittstellen, Grenzwerte, Timeouts, Ablageort der Protokolle).
 */
class Config {
public:
    Config();
    ~Config();

    /**
     * @brief Lädt die Einstellungen aus einer Datei.
     * @return true bei Erfolg
     */
    bool load(const QString &filePath);

    QString reportDirectory() const;

private:
    QString m_reportDirectory;
};

#endif //ZNI_BG_TESTING_CONFIG_H
