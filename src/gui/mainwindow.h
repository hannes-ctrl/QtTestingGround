#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include <memory>
#include <vector>

#include "core/testResult.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class Config;
class QLineEdit;
class Logger;
class TestCase;
class TestContext;
class TestRunner;
class TestSession;
class ZniConnection;

/**
 * @brief Hauptfenster: Eingabemaske, Start der Tests und Anzeige der Ergebnisse.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

    private slots:
        void on_button_clicked();
        void onTestStarted(const QString &testName);
        void onTestFinished(const TestResult &result);
        void onRunFinished();
        void onRunFailed(const QString &message);

    private:
        /**
         * @brief Erstellt die Liste der Tests anhand der gesetzten Checkboxen.
         */
        std::vector<std::unique_ptr<TestCase>> createSelectedTests() const;

        /**
         * @brief Liest eine IP-Adresse aus einem Eingabefeld; ist es leer, gilt der Platzhalter.
         * @return false bei ungültiger Adresse (Meldung in der Statusleiste)
         */
        bool readHost(const QLineEdit *input, const QString &label, QString &host);

        /**
         * @brief Simulierte Verbindung, die die eingegebene Seriennummer und je Modem einen ZLV-Status meldet.
         */
        std::unique_ptr<ZniConnection> createSimulation(const QString &serialNumber) const;

        /**
         * @brief Schreibt das JSON-Protokoll des Durchlaufs und erzeugt daraus das PDF.
         */
        void writeReports();

        Ui::MainWindow *ui;

        std::unique_ptr<Config> m_config;
        std::unique_ptr<Logger> m_logger;
        std::unique_ptr<ZniConnection> m_connection;

        std::unique_ptr<TestContext> m_context;
        std::unique_ptr<TestSession> m_session;

        QThread m_runnerThread;
        TestRunner *m_runner;
};
#endif // MAINWINDOW_H
