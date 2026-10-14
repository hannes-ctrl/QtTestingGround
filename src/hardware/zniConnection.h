#ifndef ZNI_BG_TESTING_ZNICONNECTION_H
#define ZNI_BG_TESTING_ZNICONNECTION_H

#include <QString>

#include "telegram.h"

/**
 * @brief Verbindung zum ZNI. Die Tests kennen nur dieses Interface,
 *        nicht die konkrete (echte oder simulierte) Implementierung.
 *
 * Neben den Antworten auf Kommandos schickt das ZNI auch unaufgefordert Telegramme
 * (z. B. ZLV-Status 62, empfangene ZLV- und TTY-Telegramme 7A und 45). Diese werden
 * zwischengespeichert und lassen sich mit receive() abholen.
 */
class ZniConnection {
public:
    virtual ~ZniConnection() = default;

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    /**
     * @brief Sendet ein Telegramm und wartet auf die Antwort mit demselben Kommando.
     * @param request zu sendendes Telegramm
     * @param response empfangenes Telegramm (nur bei Erfolg gültig)
     * @return true bei Erfolg, sonst steht die Ursache in errorString()
     */
    virtual bool send(const Telegram &request, Telegram &response) = 0;

    /**
     * @brief Wartet auf ein unaufgefordert gesendetes Telegramm mit dem angegebenen Kommando.
     * @return true, wenn innerhalb von timeoutMs eines eingetroffen ist
     */
    virtual bool receive(quint8 command, Telegram &telegram, int timeoutMs) = 0;

    virtual QString errorString() const = 0;
};

#endif //ZNI_BG_TESTING_ZNICONNECTION_H
