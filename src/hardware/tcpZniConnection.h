#ifndef ZNI_BG_TESTING_TCPZNICONNECTION_H
#define ZNI_BG_TESTING_TCPZNICONNECTION_H

#include <QByteArray>
#include <QString>
#include <QVector>

#include <memory>

#include "zniConnection.h"

class QTcpSocket;

/**
 * @brief Verbindung zum ZNI über Ethernet (TCP).
 *
 * Der Socket wird erst in open() angelegt und gehört damit dem Thread, der open() aufruft.
 * open(), send(), receive() und close() müssen deshalb aus demselben Thread aufgerufen werden.
 */
class TcpZniConnection : public ZniConnection {
public:
    TcpZniConnection(const QString &host, quint16 port, int timeoutMs);
    ~TcpZniConnection() override;

    bool open() override;
    void close() override;
    bool isOpen() const override;

    bool send(const Telegram &request, Telegram &response) override;
    bool receive(quint8 command, Telegram &telegram, int timeoutMs) override;

    QString errorString() const override;

private:
    /**
     * @brief Übernimmt alle bereits empfangenen Bytes und zerlegt sie in Telegramme.
     */
    void readAvailable();

    /**
     * @brief Nimmt das älteste zwischengespeicherte Telegramm mit dem Kommando heraus.
     */
    bool takePending(quint8 command, Telegram &telegram);

    /**
     * @brief Wartet, bis ein Telegramm mit dem Kommando eingetroffen ist; andere werden zwischengespeichert.
     */
    bool waitFor(quint8 command, Telegram &telegram, int timeoutMs);

    QString m_host;
    quint16 m_port;
    int m_timeoutMs;
    std::unique_ptr<QTcpSocket> m_socket;
    QByteArray m_buffer;
    QVector<Telegram> m_pending;
    QString m_errorString;
};

#endif //ZNI_BG_TESTING_TCPZNICONNECTION_H
