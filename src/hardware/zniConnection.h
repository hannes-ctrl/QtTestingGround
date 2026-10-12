#ifndef ZNI_BG_TESTING_ZNICONNECTION_H
#define ZNI_BG_TESTING_ZNICONNECTION_H

#include <QString>

#include "telegram.h"

/**
 * @brief Verbindung zum Sim-ZNI. Die Tests kennen nur dieses Interface,
 *        nicht die konkrete (echte oder simulierte) Implementierung.
 */
class ZniConnection {
public:
    virtual ~ZniConnection() = default;

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    /**
     * @brief Sendet ein Telegramm und wartet auf die Antwort.
     * @param request zu sendendes Telegramm
     * @param response empfangenes Telegramm (nur bei Erfolg gültig)
     * @return true bei Erfolg, sonst steht die Ursache in errorString()
     */
    virtual bool send(const Telegram &request, Telegram &response) = 0;

    virtual QString errorString() const = 0;
};

#endif //ZNI_BG_TESTING_ZNICONNECTION_H
