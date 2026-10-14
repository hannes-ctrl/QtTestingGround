#ifndef ZNI_BG_TESTING_TELEGRAM_H
#define ZNI_BG_TESTING_TELEGRAM_H

#include <QByteArray>
#include <QString>

/**
 * @brief Ein Telegramm fester Länge (40 Byte), wie es zwischen Test-PC und Sim-ZNI ausgetauscht wird.
 *
 * Die Byte-Indizes dieser Klasse beginnen bei 0. "Byte 3" der Testspezifikation ist also Index 2.
 */
class Telegram {
public:
    static constexpr int Size = 40;

    /**
     * @brief Erzeugt ein Telegramm aus 40 Nullbytes.
     */
    Telegram();

    /**
     * @brief Erzeugt ein Telegramm aus Rohdaten. Kürzere Daten werden mit Nullbytes aufgefüllt.
     */
    explicit Telegram(const QByteArray &data);
    ~Telegram();

    /**
     * @brief Erzeugt ein Telegramm aus Hex-Text, z. B. "73 01 01". Leerzeichen werden ignoriert,
     *        fehlende Bytes mit Nullbytes aufgefüllt.
     */
    static Telegram fromHex(const QString &hex);

    /**
     * @brief false, wenn die Daten länger als 40 Byte waren.
     */
    bool isValid() const;

    /**
     * @brief Das erste Byte (Kommando) des Telegramms.
     */
    quint8 command() const;

    quint8 byteAt(int index) const;
    void setByte(int index, quint8 value);

    /**
     * @brief Ausschnitt aus count Bytes ab index.
     */
    QByteArray bytes(int index, int count) const;

    /**
     * @brief Ausschnitt als ASCII-Text; Nullbytes und Leerzeichen am Ende werden entfernt.
     */
    QString ascii(int index, int count) const;

    /**
     * @brief Ausschnitt als vorzeichenlose Zahl, höherwertiges Byte zuerst (höchstens 4 Byte).
     */
    quint32 number(int index, int count) const;

    /**
     * @brief Fehlercode einer Antwort (Byte 40, "ER"); 00 bedeutet OK.
     */
    quint8 errorCode() const;

    /**
     * @brief Zeitstempel einer Antwort (Byte 32 bis 39, "TT").
     */
    QByteArray timestamp() const;

    QByteArray data() const;

    /**
     * @brief Hex-Darstellung in Großbuchstaben mit Leerzeichen, z. B. "73 01 01 00 ...".
     */
    QString toHex() const;

    bool operator==(const Telegram &other) const;
    bool operator!=(const Telegram &other) const;

private:
    QByteArray m_data;
};

#endif //ZNI_BG_TESTING_TELEGRAM_H
