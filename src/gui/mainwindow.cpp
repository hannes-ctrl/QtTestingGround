#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "config/config.h"
#include "core/testCase.h"
#include "core/testContext.h"
#include "core/testRunner.h"
#include "core/testSession.h"
#include "hardware/simulatedSwitchCard.h"
#include "hardware/simulatedTty.h"
#include "hardware/simulatedZlvBus.h"
#include "tests/switchCardsTest.h"
#include "tests/ttyTest.h"
#include "tests/zlvTest.h"
#include "util/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_config(std::make_unique<Config>())
    , m_logger(std::make_unique<Logger>())
    , m_zlvBus(std::make_unique<SimulatedZlvBus>())
    , m_tty(std::make_unique<SimulatedTty>())
    , m_switchCard(std::make_unique<SimulatedSwitchCard>())
    , m_context(std::make_unique<TestContext>(m_zlvBus.get(), m_tty.get(), m_switchCard.get(),
                                              m_config.get(), m_logger.get()))
    , m_runner(new TestRunner)
{
    ui->setupUi(this);

    qRegisterMetaType<TestResult>("TestResult");

    m_runner->moveToThread(&m_runnerThread);
    connect(&m_runnerThread, &QThread::finished, m_runner, &QObject::deleteLater);
    connect(m_runner, &TestRunner::testStarted, this, &MainWindow::onTestStarted);
    connect(m_runner, &TestRunner::testFinished, this, &MainWindow::onTestFinished);
    connect(m_runner, &TestRunner::runFinished, this, &MainWindow::onRunFinished);
    m_runnerThread.start();
}

MainWindow::~MainWindow()
{
    m_runner->abort();
    m_runnerThread.quit();
    m_runnerThread.wait();
    delete ui;
}

void MainWindow::on_button_clicked()
{
    ui->button->setEnabled(false);

    m_session = std::make_unique<TestSession>(ui->serialNumberInput->text());
    m_context->setSerialNumber(m_session->serialNumber());

    m_runner->setTests(createSelectedTests());
    m_runner->setContext(m_context.get());
    QMetaObject::invokeMethod(m_runner, &TestRunner::run, Qt::QueuedConnection);
}

void MainWindow::onTestStarted(const QString &testName)
{
    Q_UNUSED(testName)
}

void MainWindow::onTestFinished(const TestResult &result)
{
    m_session->addResult(result);
}

void MainWindow::onRunFinished()
{
    m_session->finish();
    ui->button->setEnabled(true);
}

std::vector<std::unique_ptr<TestCase>> MainWindow::createSelectedTests() const
{
    std::vector<std::unique_ptr<TestCase>> tests;

    if (ui->zlvCheckBox->isChecked()) {
        tests.push_back(std::make_unique<ZlvTest>());
    }
    if (ui->ttyCheckBox->isChecked()) {
        tests.push_back(std::make_unique<TtyTest>());
    }
    if (ui->switchCardsCheckBox->isChecked()) {
        tests.push_back(std::make_unique<SwitchCardsTest>());
    }

    return tests;
}
