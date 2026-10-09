#ifndef ZNI_BG_TESTING_SIMULATEDZLVBUS_H
#define ZNI_BG_TESTING_SIMULATEDZLVBUS_H

#include "zlvBusInterface.h"

/**
 * @brief Simulierter ZLV-Bus für die Entwicklung ohne Baugruppe.
 */
class SimulatedZlvBus : public ZlvBusInterface {
public:
    SimulatedZlvBus();
    ~SimulatedZlvBus() override;

    bool open() override;
    void close() override;
    bool isOpen() const override;

private:
    bool m_open;
};

#endif //ZNI_BG_TESTING_SIMULATEDZLVBUS_H
