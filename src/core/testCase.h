#ifndef ZNI_BG_TESTING_TESTCASE_H
#define ZNI_BG_TESTING_TESTCASE_H

#include <QString>

#include <functional>

#include "testResult.h"

class Telegram;
class TestContext;

/**
 * @brief Abstrakte Basisklasse aller Tests (eine abgeleitete Klasse pro Testgruppe).
 */
class TestCase {
public:
    virtual ~TestCase();

    QString name() const;

    /**
     * @brief Führt alle Prüfschritte des Tests aus.
     * @param context Zugriff auf Hardware, Konfiguration und Abbruch-Flag
     * @return Ergebnis mit einem Eintrag pro Prüfschritt
     */
    virtual TestResult run(TestContext &context) = 0;

protected:
    /**
     * @brief Zusätzliche Prüfung einer Antwort.
     * @param response die empfangene Antwort
     * @param message Meldung fürs Protokoll (Abweichung oder ausgelesene Werte)
     * @return true, wenn die Antwort den Erwartungen entspricht
     */
    using ResponseCheck = std::function<bool(const Telegram &response, QString &message)>;

    explicit TestCase(const QString &name);

    /**
     * @brief Sendet ein Telegramm und bewertet die Antwort als Prüfschritt.
     *
     * - keine Antwort: Error
     * - Fehlercode (Byte 40) ungleich 00: Failed mit der Beschreibung des Fehlercodes
     * - sonst entscheidet check (falls angegeben), ohne check: Passed
     *
     * @param response erhält die Antwort, falls angegeben und eine empfangen wurde
     */
    StepResult sendStep(TestContext &context, const QString &stepName, const Telegram &request,
                        const ResponseCheck &check = nullptr, Telegram *response = nullptr) const;

    /**
     * @brief Prüfung, dass die Antwort im dritten Byte die angefragte Peripherie zurückmeldet.
     */
    static ResponseCheck echoesThirdByte(quint8 expected);

    /**
     * @brief Prüfschritt, der laut Testspezifikation (noch) nicht ausgeführt werden kann.
     */
    static StepResult skippedStep(const QString &stepName, const QString &reason);

    /**
     * @brief Zweistellige Hex-Darstellung eines Bytes, z. B. "7A".
     */
    static QString hexByte(quint8 value);

private:
    QString m_name;
};

#endif //ZNI_BG_TESTING_TESTCASE_H
