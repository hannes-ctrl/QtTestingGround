#include "jsonReportWriter.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include "core/testSession.h"

namespace {

QJsonObject stepToJson(const StepResult &step)
{
    QJsonObject object;
    object.insert(QStringLiteral("name"), step.name());
    object.insert(QStringLiteral("status"), testStatusToString(step.status()));
    object.insert(QStringLiteral("message"), step.message());
    object.insert(QStringLiteral("sent"), step.sent());
    object.insert(QStringLiteral("received"), step.received());
    object.insert(QStringLiteral("expected"), step.expected());
    return object;
}

QJsonObject testToJson(const TestResult &result)
{
    QJsonArray steps;
    for (const StepResult &step : result.steps()) {
        steps.append(stepToJson(step));
    }

    QJsonObject object;
    object.insert(QStringLiteral("name"), result.testName());
    object.insert(QStringLiteral("status"), testStatusToString(result.status()));
    object.insert(QStringLiteral("durationMs"), result.durationMs());
    object.insert(QStringLiteral("steps"), steps);
    return object;
}

/**
 * @brief Macht aus der Seriennummer einen gültigen Bestandteil eines Dateinamens.
 */
QString fileNamePart(const QString &serialNumber)
{
    QString part = serialNumber.trimmed();
    part.replace(QRegularExpression(QStringLiteral("[^A-Za-z0-9_-]")), QStringLiteral("_"));
    return part.isEmpty() ? QStringLiteral("ohne-Seriennummer") : part;
}

}

JsonReportWriter::JsonReportWriter(const QString &outputDirectory)
    : m_outputDirectory(outputDirectory)
{
}

JsonReportWriter::~JsonReportWriter() = default;

QString JsonReportWriter::write(const TestSession &session)
{
    QJsonArray tests;
    for (const TestResult &result : session.results()) {
        tests.append(testToJson(result));
    }

    QJsonObject root;
    root.insert(QStringLiteral("serialNumber"), session.serialNumber());
    root.insert(QStringLiteral("softwareVersion"), QCoreApplication::applicationVersion());
    root.insert(QStringLiteral("startedAt"), session.startedAt().toString(Qt::ISODate));
    root.insert(QStringLiteral("finishedAt"), session.finishedAt().toString(Qt::ISODate));
    root.insert(QStringLiteral("status"), testStatusToString(session.status()));
    root.insert(QStringLiteral("tests"), tests);

    if (!QDir().mkpath(m_outputDirectory)) {
        m_errorString = QStringLiteral("Verzeichnis %1 konnte nicht angelegt werden").arg(m_outputDirectory);
        return QString();
    }

    const QString fileName = QStringLiteral("%1_%2.json")
                                 .arg(fileNamePart(session.serialNumber()),
                                      session.startedAt().toString(QStringLiteral("yyyy-MM-dd_HH-mm-ss")));
    const QString filePath = QDir(m_outputDirectory).filePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        m_errorString = QStringLiteral("%1 konnte nicht geschrieben werden (%2)").arg(filePath, file.errorString());
        return QString();
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));

    m_errorString.clear();
    return filePath;
}

QString JsonReportWriter::errorString() const
{
    return m_errorString;
}
