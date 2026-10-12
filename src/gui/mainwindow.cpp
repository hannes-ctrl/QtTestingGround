#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>

#include "config/config.h"
#include "core/testCase.h"
#include "core/testContext.h"
#include "core/testRunner.h"
#include "core/testSession.h"
#include "hardware/simulatedZniConnection.h"
#include "hardware/tcpZniConnection.h"
#include "report/jsonReportWriter.h"
#include "report/pdfReportGenerator.h"
#include "tests/switchCardsTest.h"
#include "tests/ttyTest.h"
#include "tests/zlvTest.h"
#include "util/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_config(std::make_unique<Config>())
    , m_logger(std::make_unique<Logger>())
    , m_runner(new TestRunner)
{
    ui->setupUi(this);

    // Ohne config.json neben der Anwendung gelten die Standardwerte
    m_config->load(QCoreApplication::applicationDirPath() + QStringLiteral("/config.json"));

    if (m_config->simulation()) {
        m_connection = std::make_unique<SimulatedZniConnection>();
    } else {
        m_connection = std::make_unique<TcpZniConnection>(m_config->host(), m_config->port(), m_config->timeoutMs());
    }
    m_context = std::make_unique<TestContext>(m_connection.get(), m_config.get(), m_logger.get());

    qRegisterMetaType<TestResult>("TestResult");

    m_runner->moveToThread(&m_runnerThread);
    connect(&m_runnerThread, &QThread::finished, m_runner, &QObject::deleteLater);
    connect(m_runner, &TestRunner::testStarted, this, &MainWindow::onTestStarted);
    connect(m_runner, &TestRunner::testFinished, this, &MainWindow::onTestFinished);
    connect(m_runner, &TestRunner::runFinished, this, &MainWindow::onRunFinished);
    connect(m_runner, &TestRunner::runFailed, this, &MainWindow::onRunFailed);
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
    ui->statusbar->showMessage(QStringLiteral("Läuft: %1").arg(testName));
}

void MainWindow::onTestFinished(const TestResult &result)
{
    m_session->addResult(result);
}

void MainWindow::onRunFinished()
{
    m_session->finish();
    writeReports();
    ui->button->setEnabled(true);
}

void MainWindow::onRunFailed(const QString &message)
{
    ui->statusbar->showMessage(QStringLiteral("Tests nicht gestartet: %1").arg(message));
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

void MainWindow::writeReports()
{
    JsonReportWriter jsonWriter(m_config->reportDirectory());
    const QString jsonFilePath = jsonWriter.write(*m_session);
    if (jsonFilePath.isEmpty()) {
        ui->statusbar->showMessage(QStringLiteral("Protokoll nicht geschrieben: %1").arg(jsonWriter.errorString()));
        return;
    }

    PdfReportGenerator pdfGenerator;
    const QString pdfFilePath = pdfGenerator.generate(jsonFilePath);
    if (pdfFilePath.isEmpty()) {
        ui->statusbar->showMessage(QStringLiteral("PDF nicht erzeugt: %1").arg(pdfGenerator.errorString()));
        return;
    }

    ui->statusbar->showMessage(QStringLiteral("Protokoll: %1").arg(QDir::toNativeSeparators(pdfFilePath)));
}
