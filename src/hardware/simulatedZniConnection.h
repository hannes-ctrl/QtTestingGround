#ifndef ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H
#define ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H

#include <QHash>

#include "zniConnection.h"

/**
 * @brief Simulierte Verbindung für die Entwicklung ohne Testrack.
 *
 * Antwortet je Kommando (erstes Byte) mit einem hinterlegten Telegramm.
 * Ist für ein Kommando nichts hinterlegt, wird das gesendete Telegramm zurückgegeben.
 */
class SimulatedZniConnection : public ZniConnection {
public:
    SimulatedZniConnection();
    ~SimulatedZniConnection() override;

    /**
     * @brief Hinterlegt die Antwort für ein Kommando.
     */
    void setResponse(quint8 command, const Telegram &response);

    bool open() override;
    void close() override;
    bool isOpen() const override;

    bool send(const Telegram &request, Telegram &response) override;

    QString errorString() const override;

private:
    bool m_open;
    QHash<quint8, Telegram> m_responses;
    QString m_errorString;
};

#endif //ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H
