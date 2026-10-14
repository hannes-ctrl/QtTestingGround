#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QHostAddress>
#include <QLineEdit>

#include "config/config.h"
#include "core/testCase.h"
#include "core/testContext.h"
#include "core/testRunner.h"
#include "core/testSession.h"
#include "hardware/simulatedZniConnection.h"
#include "hardware/tcpZniConnection.h"
#include "hardware/telegramBuilder.h"
#include "report/jsonReportWriter.h"
#include "report/pdfReportGenerator.h"
#include "tests/initialTest.h"
#include "tests/miscTest.h"
#include "tests/switchCardsTest.h"
#include "tests/ttyTest.h"
#include "tests/zlvTest.h"
#include "util/logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_config(std::make_unique<Config>())
    , m_logger(std::make_unique<Logger>())
    , m_context(std::make_unique<TestContext>(nullptr, m_config.get(), m_logger.get()))
    , m_runner(new TestRunner)
{
    ui->setupUi(this);

    // Ohne config.json neben der Anwendung gelten die Standardwerte
    m_config->load(QCoreApplication::applicationDirPath() + QStringLiteral("/config.json"));

    // Die Platzhalter zeigen die Adressen, die bei leerem Eingabefeld verwendet werden
    ui->ipSimZniInput->setPlaceholderText(m_config->host());
    ui->ipTestZniInput->setPlaceholderText(m_config->testZniHost());

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
    QString simZniHost;
    QString testZniHost;
    if (!readHost(ui->ipSimZniInput, QStringLiteral("Sim-ZNI"), simZniHost)
        || !readHost(ui->ipTestZniInput, QStringLiteral("Test-ZNI"), testZniHost)) {
        return;
    }

    ui->button->setEnabled(false);

    m_session = std::make_unique<TestSession>(ui->serialNumberInput->text());

    // Die Verbindung wird für jeden Durchlauf neu angelegt, weil sich die Adresse geändert haben kann.
    // Laut Testspezifikation gehen alle Telegramme an das zu testende ZNI.
    if (m_config->simulation()) {
        m_connection = createSimulation(m_session->serialNumber());
    } else {
        m_connection = std::make_unique<TcpZniConnection>(testZniHost, m_config->port(), m_config->timeoutMs());
    }

    m_context->setConnection(m_connection.get());
    m_context->setSerialNumber(m_session->serialNumber());
    m_context->setTestZniHost(testZniHost);
    m_context->setSimZniHost(simZniHost);

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

bool MainWindow::readHost(const QLineEdit *input, const QString &label, QString &host)
{
    host = input->text().trimmed();
    if (host.isEmpty()) {
        host = input->placeholderText();
    }

    if (QHostAddress(host).isNull()) {
        ui->statusbar->showMessage(QStringLiteral("IP-Adresse %1 ist ungültig: %2").arg(label, host));
        return false;
    }
    return true;
}

std::unique_ptr<ZniConnection> MainWindow::createSimulation(const QString &serialNumber) const
{
    auto simulation = std::make_unique<SimulatedZniConnection>();

    // Seriennummer so zurückmelden, wie sie eingegeben wurde
    Telegram serialNumberResponse = TelegramBuilder::status(TelegramBuilder::StatusPageSerialNumber);
    const QByteArray serialNumberText = serialNumber.trimmed().toLatin1().left(29);
    for (int i = 0; i < serialNumberText.size(); ++i) {
        serialNumberResponse.setByte(2 + i, static_cast<quint8>(serialNumberText.at(i)));
    }
    simulation->setResponse(static_cast<quint8>(ZniCommand::Status), TelegramBuilder::StatusPageSerialNumber,
                            serialNumberResponse);

    // Je Modem eine Statusmeldung "regulärer Betrieb" bereitlegen
    const ZniChannel modems[] = {ZniChannel::Modem1, ZniChannel::Modem2, ZniChannel::Modem3, ZniChannel::Modem4};
    for (const ZniChannel modem : modems) {
        Telegram status;
        status.setByte(0, static_cast<quint8>(ZniCommand::ZlvStatus));
        status.setByte(1, 0x01);
        status.setByte(2, static_cast<quint8>(modem));
        status.setByte(3, 0x01);
        simulation->queueTelegram(status);
    }

    return simulation;
}

std::vector<std::unique_ptr<TestCase>> MainWindow::createSelectedTests() const
{
    std::vector<std::unique_ptr<TestCase>> tests;

    // Die ersten Tests laufen immer und vor allen anderen
    tests.push_back(std::make_unique<InitialTest>());

    if (ui->zlvCheckBox->isChecked()) {
        tests.push_back(std::make_unique<ZlvTest>());
    }
    if (ui->ttyCheckBox->isChecked()) {
        tests.push_back(std::make_unique<TtyTest>());
    }
    if (ui->switchCardsCheckBox->isChecked()) {
        tests.push_back(std::make_unique<SwitchCardsTest>());
    }
    // Muss der letzte Test bleiben, weil er mit dem Reset endet
    if (ui->miscCheckBox->isChecked()) {
        tests.push_back(std::make_unique<MiscTest>());
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
