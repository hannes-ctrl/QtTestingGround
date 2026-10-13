#ifndef ZNI_BG_TESTING_ZNIERRORCODE_H
#define ZNI_BG_TESTING_ZNIERRORCODE_H

#include <QString>

/**
 * @brief Fehlercodes des ZNI laut Testspezifikation, Kapitel "Fehlercodes".
 */
class ZniErrorCode {
public:
    /**
     * @brief true, wenn der Code in der Tabelle der Testspezifikation steht.
     */
    static bool isKnown(quint8 code);

    /**
     * @brief Beschreibung des Fehlercodes fürs Protokoll, z. B. "Kanal öffnen: ungültiger Kanal".
     *        Für unbekannte Codes "Unbekannter Fehlercode XX".
     */
    static QString description(quint8 code);
};

#endif //ZNI_BG_TESTING_ZNIERRORCODE_H
