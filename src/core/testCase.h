#ifndef ZNI_BG_TESTING_TESTCASE_H
#define ZNI_BG_TESTING_TESTCASE_H

#include <QString>

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
    explicit TestCase(const QString &name);

    /**
     * @brief Sendet ein Telegramm und macht daraus einen Prüfschritt mit gesendetem und empfangenem Telegramm.
     *
     * Kommt keine Antwort, hat der Schritt den Status Error. Sonst ist er NotEvaluated,
     * solange die Auswertung der Antwort nicht festgelegt ist.
     *
     * @param response erhält die Antwort, falls angegeben und eine empfangen wurde
     */
    StepResult sendStep(TestContext &context, const QString &stepName, const Telegram &request,
                        Telegram *response = nullptr) const;

    /**
     * @brief Prüfschritt, der laut Testspezifikation (noch) nicht ausgeführt werden kann.
     */
    static StepResult skippedStep(const QString &stepName, const QString &reason);

private:
    QString m_name;
};

#endif //ZNI_BG_TESTING_TESTCASE_H
