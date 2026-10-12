#ifndef ZNI_BG_TESTING_TESTCONTEXT_H
#define ZNI_BG_TESTING_TESTCONTEXT_H

#include <QString>

#include <atomic>

class Config;
class Logger;
class ZniConnection;

/**
 * @brief Bündelt alles, was ein Test zur Ausführung braucht. Besitzt die Objekte nicht.
 */
class TestContext {
public:
    TestContext(ZniConnection *connection, Config *config, Logger *logger);
    ~TestContext();

    ZniConnection *connection() const;
    Config *config() const;
    Logger *logger() const;

    QString serialNumber() const;
    void setSerialNumber(const QString &serialNumber);

    bool abortRequested() const;
    void setAbortRequested(bool abortRequested);

private:
    ZniConnection *m_connection;
    Config *m_config;
    Logger *m_logger;

    QString m_serialNumber;
    std::atomic_bool m_abortRequested;
};

#endif //ZNI_BG_TESTING_TESTCONTEXT_H
