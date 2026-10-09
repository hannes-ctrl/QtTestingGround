#ifndef ZNI_BG_TESTING_TESTCASE_H
#define ZNI_BG_TESTING_TESTCASE_H

#include <QString>

#include "testResult.h"

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

private:
    QString m_name;
};

#endif //ZNI_BG_TESTING_TESTCASE_H
