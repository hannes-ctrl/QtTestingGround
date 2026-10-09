#ifndef ZNI_BG_TESTING_SIMULATEDTTY_H
#define ZNI_BG_TESTING_SIMULATEDTTY_H

#include "ttyInterface.h"

/**
 * @brief Simulierte TTY-Schnittstellen für die Entwicklung ohne Baugruppe.
 */
class SimulatedTty : public TtyInterface {
public:
    SimulatedTty();
    ~SimulatedTty() override;

    bool open() override;
    void close() override;
    bool isOpen() const override;

private:
    bool m_open;
};

#endif //ZNI_BG_TESTING_SIMULATEDTTY_H
