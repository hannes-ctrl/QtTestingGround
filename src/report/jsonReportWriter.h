#ifndef ZNI_BG_TESTING_JSONREPORTWRITER_H
#define ZNI_BG_TESTING_JSONREPORTWRITER_H

#include <QString>

class TestSession;

/**
 * @brief Schreibt die Ergebnisse eines Durchlaufs als maschinenlesbare JSON-Datei.
 *
 * Die JSON-Datei ist die Quelle für alle weiteren Ausgaben (z. B. das PDF-Protokoll).
 */
class JsonReportWriter {
public:
    explicit JsonReportWriter(const QString &outputDirectory);
    ~JsonReportWriter();

    /**
     * @brief Schreibt die Datei "<Seriennummer>_<Startzeitpunkt>.json" in das Ausgabeverzeichnis.
     * @return Pfad der geschriebenen Datei, bei einem Fehler ein leerer String (Ursache in errorString())
     */
    QString write(const TestSession &session);

    QString errorString() const;

private:
    QString m_outputDirectory;
    QString m_errorString;
};

#endif //ZNI_BG_TESTING_JSONREPORTWRITER_H
