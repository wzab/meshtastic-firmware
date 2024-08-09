#pragma once
#include "kbI2cBase.h"

/**
 * @brief The idea behind this class to have static methods for the event handlers.
 *      Check attachInterrupt() at RotaryEncoderInteruptBase.cpp
 *      Technically you can have as many rotary encoders hardver attached
 *      to your device as you wish, but you always need to have separate event
 *      handlers, thus you need to have a RotaryEncoderInterrupt implementation.
 */

#define MPR121_LED 13
#define MPR121_IRQ 15

class Mpr121KbI2cImpl : public KbI2cBase
{
  public:
    Mpr121KbI2cImpl();
    void init();
    void reg8_write(uint8_t addr, uint8_t val);
    uint8_t reg8_read(uint8_t addr);
    uint16_t reg16_read(uint8_t addr);
    void set_thresholds(uint8_t touch, uint8_t release);
    uint8_t baseline_data(int electrode);
    uint16_t filtered_data(int electrode);
    uint16_t touched();
    int is_touched(int electrode);
    void reset();
    static void handleInt();
    void intHandler();
};

extern Mpr121KbI2cImpl *mpr121KbI2cImpl;
