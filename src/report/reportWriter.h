#ifndef ZNI_BG_TESTING_REPORTWRITER_H
#define ZNI_BG_TESTING_REPORTWRITER_H

#include <QString>

class TestSession;

/**
 * @brief Schreibt das Prüfprotokoll eines Durchlaufs in eine Datei.
 */
class ReportWriter {
public:
    explicit ReportWriter(const QString &outputDirectory);
    ~ReportWriter();

    /**
     * @brief Schreibt das Protokoll für den übergebenen Durchlauf.
     * @return true bei Erfolg
     */
    bool write(const TestSession &session);

private:
    QString m_outputDirectory;
};

#endif //ZNI_BG_TESTING_REPORTWRITER_H
