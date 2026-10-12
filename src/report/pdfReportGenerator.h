#ifndef ZNI_BG_TESTING_PDFREPORTGENERATOR_H
#define ZNI_BG_TESTING_PDFREPORTGENERATOR_H

#include <QString>

class QJsonObject;

/**
 * @brief Erzeugt das PDF-Prüfprotokoll aus einer JSON-Datei des JsonReportWriter.
 *
 * Arbeitet nur auf der JSON-Datei, nicht auf den Objekten im Speicher. Ein Protokoll
 * lässt sich dadurch jederzeit nachträglich neu erzeugen.
 */
class PdfReportGenerator {
public:
    PdfReportGenerator();
    ~PdfReportGenerator();

    /**
     * @brief Erzeugt das PDF neben der JSON-Datei (gleicher Name, Endung .pdf).
     * @return Pfad der geschriebenen Datei, bei einem Fehler ein leerer String (Ursache in errorString())
     */
    QString generate(const QString &jsonFilePath);

    QString errorString() const;

private:
    /**
     * @brief Baut den Inhalt des Protokolls als HTML auf.
     */
    QString buildHtml(const QJsonObject &root) const;

    QString m_errorString;
};

#endif //ZNI_BG_TESTING_PDFREPORTGENERATOR_H
