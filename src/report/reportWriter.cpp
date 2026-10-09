#include "reportWriter.h"

#include "core/testSession.h"

ReportWriter::ReportWriter(const QString &outputDirectory)
    : m_outputDirectory(outputDirectory)
{
}

ReportWriter::~ReportWriter() = default;

bool ReportWriter::write(const TestSession &session)
{
    Q_UNUSED(session)

    // TODO: Protokollformat festlegen und Datei schreiben
    return false;
}
