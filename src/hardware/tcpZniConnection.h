#ifndef ZNI_BG_TESTING_TCPZNICONNECTION_H
#define ZNI_BG_TESTING_TCPZNICONNECTION_H

#include <QString>

#include <memory>

#include "zniConnection.h"

class QTcpSocket;

/**
 * @brief Verbindung zum Sim-ZNI über Ethernet (TCP).
 *
 * Der Socket wird erst in open() angelegt und gehört damit dem Thread, der open() aufruft.
 * open(), send() und close() müssen deshalb aus demselben Thread aufgerufen werden.
 */
class TcpZniConnection : public ZniConnection {
public:
    TcpZniConnection(const QString &host, quint16 port, int timeoutMs);
    ~TcpZniConnection() override;

    bool open() override;
    void close() override;
    bool isOpen() const override;

    bool send(const Telegram &request, Telegram &response) override;

    QString errorString() const override;

private:
    QString m_host;
    quint16 m_port;
    int m_timeoutMs;
    std::unique_ptr<QTcpSocket> m_socket;
    QString m_errorString;
};

#endif //ZNI_BG_TESTING_TCPZNICONNECTION_H
