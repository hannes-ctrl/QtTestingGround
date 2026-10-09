#ifndef ZNI_BG_TESTING_TTYINTERFACE_H
#define ZNI_BG_TESTING_TTYINTERFACE_H

#include "hardwareInterface.h"

/**
 * @brief Zugriff auf die TTY-Schnittstellen. Die Tests kennen nur dieses Interface,
 *        nicht die konkrete (echte oder simulierte) Implementierung.
 */
class TtyInterface : public HardwareInterface {
public:
    ~TtyInterface() override = default;

    // TODO: schnittstellenspezifische Operationen als rein virtuelle Methoden
};

#endif //ZNI_BG_TESTING_TTYINTERFACE_H
