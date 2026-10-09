#ifndef ZNI_BG_TESTING_HARDWAREINTERFACE_H
#define ZNI_BG_TESTING_HARDWAREINTERFACE_H

/**
 * @brief Gemeinsame Basis aller Hardware-Schnittstellen zur Baugruppe.
 */
class HardwareInterface {
public:
    virtual ~HardwareInterface() = default;

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
};

#endif //ZNI_BG_TESTING_HARDWAREINTERFACE_H
