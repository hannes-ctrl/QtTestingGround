#ifndef ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H
#define ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H

#include <QHash>
#include <QVector>

#include "zniConnection.h"

/**
 * @brief Simulierte Verbindung für die Entwicklung ohne Testrack.
 *
 * Antwortet mit einem hinterlegten Telegramm. Gesucht wird zuerst nach Kommando und
 * drittem Byte, dann nur nach Kommando. Ist nichts hinterlegt, wird das gesendete
 * Telegramm zurückgegeben (Fehlercode 00).
 */
class SimulatedZniConnection : public ZniConnection {
public:
    SimulatedZniConnection();
    ~SimulatedZniConnection() override;

    /**
     * @brief Hinterlegt die Antwort für ein Kommando.
     */
    void setResponse(quint8 command, const Telegram &response);

    /**
     * @brief Hinterlegt die Antwort für ein Kommando mit einem bestimmten dritten Byte (z. B. Statusseite).
     */
    void setResponse(quint8 command, quint8 thirdByte, const Telegram &response);

    /**
     * @brief Legt ein Telegramm bereit, das receive() wie ein unaufgefordert gesendetes liefert.
     */
    void queueTelegram(const Telegram &telegram);

    bool open() override;
    void close() override;
    bool isOpen() const override;

    bool send(const Telegram &request, Telegram &response) override;
    bool receive(quint8 command, Telegram &telegram, int timeoutMs) override;

    QString errorString() const override;

private:
    bool m_open;
    QHash<quint8, Telegram> m_responses;
    QHash<quint16, Telegram> m_selectiveResponses;
    QVector<Telegram> m_queue;
    QString m_errorString;
};

#endif //ZNI_BG_TESTING_SIMULATEDZNICONNECTION_H
