#include "mpr121KbI2cImpl.h"
#include "InputBroker.h"
#include "detect/ScanI2CTwoWire.h"
#include "main.h"

Mpr121KbI2cImpl *mpr121KbI2cImpl;

Mpr121KbI2cImpl::Mpr121KbI2cImpl() : KbI2cBase("mpr121KB") {}

void Mpr121KbI2cImpl::init()
{
#ifndef ARCH_PORTDUINO
    if (cardkb_found.address == 0x00) {
        LOG_DEBUG("Rescanning for I2C keyboard\n");
        uint8_t i2caddr_scan[] = {CARDKB_ADDR, MPR121_KB_ADDR, TDECK_KB_ADDR, BBQ10_KB_ADDR};
        uint8_t i2caddr_asize = 3;
        auto i2cScanner = std::unique_ptr<ScanI2CTwoWire>(new ScanI2CTwoWire());

#if defined(I2C_SDA1)
        i2cScanner->scanPort(ScanI2C::I2CPort::WIRE1, i2caddr_scan, i2caddr_asize);
#endif
        i2cScanner->scanPort(ScanI2C::I2CPort::WIRE, i2caddr_scan, i2caddr_asize);
        auto kb_info = i2cScanner->firstKeyboard();

        if (kb_info.type != ScanI2C::DeviceType::NONE) {
            cardkb_found = kb_info.address;
            switch (kb_info.type) {
            case ScanI2C::DeviceType::RAK14004:
                kb_model = 0x02;
                break;
            case ScanI2C::DeviceType::CARDKB:
                kb_model = 0x00;
                break;
            case ScanI2C::DeviceType::TDECKKB:
                // assign an arbitrary value to distinguish from other models
                kb_model = 0x10;
                break;
            case ScanI2C::DeviceType::BBQ10KB:
                // assign an arbitrary value to distinguish from other models
                kb_model = 0x11;
                break;
            case ScanI2C::DeviceType::MPR121KB:
                // assign an arbitrary value to distinguish from other models
                kb_model = 0x14;
                break;
            default:
                // use this as default since it's also just zero
                LOG_WARN("kb_info.type is unknown(0x%02x), setting kb_model=0x00\n", kb_info.type);
                kb_model = 0x00;
            }
        }
        if (cardkb_found.address == 0x00) {
            disable();
            return;
        }
    }
#else
    if (cardkb_found.address == 0x00) {
        disable();
        return;
    }
#endif
    // Prepare the keyboard for operation
    reset();
    // Configure the keyboard LED
    pinMode(MPR121_LED, OUTPUT);
    // Connect the keyboard interrupt
    pinMode(MPR121_IRQ, INPUT_PULLUP);
    attachInterrupt(MPR121_IRQ, handleInt, FALLING);
    inputBroker->registerSource(this);
}

void Mpr121KbI2cImpl::handleInt()
{
    mpr121KbI2cImpl->intHandler();
}

void Mpr121KbI2cImpl::intHandler() {}
