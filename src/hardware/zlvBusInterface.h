#ifndef ZNI_BG_TESTING_ZLVBUSINTERFACE_H
#define ZNI_BG_TESTING_ZLVBUSINTERFACE_H

#include "hardwareInterface.h"

/**
 * @brief Zugriff auf den ZLV-Bus. Die Tests kennen nur dieses Interface,
 *        nicht die konkrete (echte oder simulierte) Implementierung.
 */
class ZlvBusInterface : public HardwareInterface {
public:
    ~ZlvBusInterface() override = default;

    // TODO: busspezifische Operationen (senden, empfangen, ...) als rein virtuelle Methoden
};

#endif //ZNI_BG_TESTING_ZLVBUSINTERFACE_H
