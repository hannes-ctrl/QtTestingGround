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
class Logger;
class TestCase;
class TestContext;
class TestRunner;
class TestSession;
class ZlvBusInterface;
class TtyInterface;
class SwitchCardInterface;

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

    private:
        /**
         * @brief Erstellt die Liste der Tests anhand der gesetzten Checkboxen.
         */
        std::vector<std::unique_ptr<TestCase>> createSelectedTests() const;

        Ui::MainWindow *ui;

        std::unique_ptr<Config> m_config;
        std::unique_ptr<Logger> m_logger;

        std::unique_ptr<ZlvBusInterface> m_zlvBus;
        std::unique_ptr<TtyInterface> m_tty;
        std::unique_ptr<SwitchCardInterface> m_switchCard;

        std::unique_ptr<TestContext> m_context;
        std::unique_ptr<TestSession> m_session;

        QThread m_runnerThread;
        TestRunner *m_runner;
};
#endif // MAINWINDOW_H
