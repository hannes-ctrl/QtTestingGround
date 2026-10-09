#ifndef ZNI_BG_TESTING_SIMULATEDSWITCHCARD_H
#define ZNI_BG_TESTING_SIMULATEDSWITCHCARD_H

#include "switchCardInterface.h"

/**
 * @brief Simulierte Anschaltkarten für die Entwicklung ohne Baugruppe.
 */
class SimulatedSwitchCard : public SwitchCardInterface {
public:
    SimulatedSwitchCard();
    ~SimulatedSwitchCard() override;

    bool open() override;
    void close() override;
    bool isOpen() const override;

private:
    bool m_open;
};

#endif //ZNI_BG_TESTING_SIMULATEDSWITCHCARD_H
