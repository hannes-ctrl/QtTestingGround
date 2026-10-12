#include "pdfReportGenerator.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMarginsF>
#include <QPageSize>
#include <QPdfWriter>
#include <QTextDocument>

namespace {

QString statusText(const QString &status)
{
    if (status == QLatin1String("passed")) {
        return QStringLiteral("bestanden");
    }
    if (status == QLatin1String("failed")) {
        return QStringLiteral("fehlgeschlagen");
    }
    if (status == QLatin1String("skipped")) {
        return QStringLiteral("übersprungen");
    }
    if (status == QLatin1String("error")) {
        return QStringLiteral("Fehler");
    }
    return QStringLiteral("nicht ausgeführt");
}

QString statusColor(const QString &status)
{
    if (status == QLatin1String("passed")) {
        return QStringLiteral("#1b7f3b");
    }
    if (status == QLatin1String("failed") || status == QLatin1String("error")) {
        return QStringLiteral("#b3261e");
    }
    return QStringLiteral("#555555");
}

QString statusHtml(const QString &status)
{
    return QStringLiteral("<span style=\"color:%1;\"><b>%2</b></span>").arg(statusColor(status), statusText(status));
}

QString dateTimeText(const QString &isoDateTime)
{
    const QDateTime dateTime = QDateTime::fromString(isoDateTime, Qt::ISODate);
    return dateTime.isValid() ? dateTime.toString(QStringLiteral("dd.MM.yyyy HH:mm:ss")) : isoDateTime;
}

/**
 * @brief Zeile mit einem Telegramm unterhalb eines Prüfschritts; leer, wenn kein Telegramm vorliegt.
 */
QString telegramRow(const QString &label, const QString &hex)
{
    if (hex.isEmpty()) {
        return QString();
    }
    return QStringLiteral("<tr><td></td><td colspan=\"2\" style=\"font-family:'Courier New'; font-size:7pt;\">"
                          "%1: %2</td></tr>").arg(label, hex.toHtmlEscaped());
}

}

PdfReportGenerator::PdfReportGenerator() = default;

PdfReportGenerator::~PdfReportGenerator() = default;

QString PdfReportGenerator::generate(const QString &jsonFilePath)
{
    QFile jsonFile(jsonFilePath);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        m_errorString = QStringLiteral("%1 konnte nicht gelesen werden (%2)").arg(jsonFilePath, jsonFile.errorString());
        return QString();
    }

    const QJsonDocument json = QJsonDocument::fromJson(jsonFile.readAll());
    if (!json.isObject()) {
        m_errorString = QStringLiteral("%1 enthält kein gültiges JSON").arg(jsonFilePath);
        return QString();
    }

    const QFileInfo jsonInfo(jsonFilePath);
    const QString pdfFilePath = jsonInfo.path() + QLatin1Char('/') + jsonInfo.completeBaseName() + QStringLiteral(".pdf");

    // Vorab prüfen, weil QPdfWriter selbst keinen Fehler meldet (z. B. wenn das PDF noch geöffnet ist)
    QFile pdfFile(pdfFilePath);
    if (!pdfFile.open(QIODevice::WriteOnly)) {
        m_errorString = QStringLiteral("%1 konnte nicht geschrieben werden (%2)").arg(pdfFilePath, pdfFile.errorString());
        return QString();
    }
    pdfFile.close();

    QPdfWriter writer(pdfFilePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    writer.setTitle(QStringLiteral("Prüfprotokoll ZNI-BG"));

    QTextDocument document;
    document.setDefaultFont(QFont(QStringLiteral("Arial"), 10));
    document.setHtml(buildHtml(json.object()));
    document.print(&writer);

    m_errorString.clear();
    return pdfFilePath;
}

QString PdfReportGenerator::errorString() const
{
    return m_errorString;
}

QString PdfReportGenerator::buildHtml(const QJsonObject &root) const
{
    QString html;
    html += QStringLiteral("<h1>Prüfprotokoll ZNI-BG</h1>");

    html += QStringLiteral("<table cellpadding=\"3\">");
    html += QStringLiteral("<tr><td>Seriennummer:</td><td><b>%1</b></td></tr>")
                .arg(root.value(QStringLiteral("serialNumber")).toString().toHtmlEscaped());
    html += QStringLiteral("<tr><td>Beginn:</td><td>%1</td></tr>")
                .arg(dateTimeText(root.value(QStringLiteral("startedAt")).toString()));
    html += QStringLiteral("<tr><td>Ende:</td><td>%1</td></tr>")
                .arg(dateTimeText(root.value(QStringLiteral("finishedAt")).toString()));
    html += QStringLiteral("<tr><td>Softwareversion:</td><td>%1</td></tr>")
                .arg(root.value(QStringLiteral("softwareVersion")).toString().toHtmlEscaped());
    html += QStringLiteral("<tr><td>Gesamtergebnis:</td><td>%1</td></tr>")
                .arg(statusHtml(root.value(QStringLiteral("status")).toString()));
    html += QStringLiteral("</table>");

    const QJsonArray tests = root.value(QStringLiteral("tests")).toArray();
    for (const QJsonValue &testValue : tests) {
        const QJsonObject test = testValue.toObject();

        html += QStringLiteral("<h2>%1</h2>").arg(test.value(QStringLiteral("name")).toString().toHtmlEscaped());
        html += QStringLiteral("<p>Ergebnis: %1, Dauer: %2 ms</p>")
                    .arg(statusHtml(test.value(QStringLiteral("status")).toString()))
                    .arg(test.value(QStringLiteral("durationMs")).toInt());

        const QJsonArray steps = test.value(QStringLiteral("steps")).toArray();
        if (steps.isEmpty()) {
            html += QStringLiteral("<p>Keine Prüfschritte ausgeführt.</p>");
            continue;
        }

        html += QStringLiteral("<table width=\"100%\" cellpadding=\"3\" cellspacing=\"0\">");
        html += QStringLiteral("<tr style=\"background-color:#e6e6e6;\">"
                               "<th align=\"left\" width=\"36%\">Prüfschritt</th>"
                               "<th align=\"left\" width=\"24%\">Ergebnis</th>"
                               "<th align=\"left\" width=\"40%\">Meldung</th></tr>");
        for (const QJsonValue &stepValue : steps) {
            const QJsonObject step = stepValue.toObject();

            html += QStringLiteral("<tr><td>%1</td><td>%2</td><td>%3</td></tr>")
                        .arg(step.value(QStringLiteral("name")).toString().toHtmlEscaped(),
                             statusHtml(step.value(QStringLiteral("status")).toString()),
                             step.value(QStringLiteral("message")).toString().toHtmlEscaped());
            html += telegramRow(QStringLiteral("Gesendet"), step.value(QStringLiteral("sent")).toString());
            html += telegramRow(QStringLiteral("Empfangen"), step.value(QStringLiteral("received")).toString());
            html += telegramRow(QStringLiteral("Erwartet"), step.value(QStringLiteral("expected")).toString());
        }
        html += QStringLiteral("</table>");
    }

    return html;
}
