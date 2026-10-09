#ifndef ZNI_BG_TESTING_SWITCHCARDINTERFACE_H
#define ZNI_BG_TESTING_SWITCHCARDINTERFACE_H

#include "hardwareInterface.h"

/**
 * @brief Zugriff auf die Anschaltkarten. Die Tests kennen nur dieses Interface,
 *        nicht die konkrete (echte oder simulierte) Implementierung.
 */
class SwitchCardInterface : public HardwareInterface {
public:
    ~SwitchCardInterface() override = default;

    // TODO: kartenspezifische Operationen als rein virtuelle Methoden
};

#endif //ZNI_BG_TESTING_SWITCHCARDINTERFACE_H
