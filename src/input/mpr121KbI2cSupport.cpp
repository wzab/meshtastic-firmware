/*
This code has been rewritten from Python in C by Wojciech M. Zabolotny in 2024,
and published under MIT license (as the original Python code).

Original license of the Python code follows:

MicroPython MPR121 capacitive touch keypad and breakout board driver
https://github.com/mcauser/micropython-mpr121

MIT License
Copyright (c) 2018 Mike Causer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include "mpr121KbI2cImpl.h"

#define MPR121_TOUCH_STATUS (0x00) // (0x00~0x01) Touch status
// (0x02~0x03) Out-of-range status
#define MPR121_ELECTRODE_FILTERED_DATA (0x04) // (0x04~0x1D) Electrode filtered data
#define MPR121_BASELINE_VALUE (0x1E)          // (0x1E~0x2A) Baseline value
// (0x2B~0x40) Baseline Filtering Control
#define MPR121_MAX_HALF_DELTA_RISING (0x2B)      // Max half delta (rising)
#define MPR121_NOISE_HALF_DELTA_RISING (0x2C)    // Noise half delta (rising)
#define MPR121_NOISE_COUNT_LIMIT_RISING (0x2D)   // Noise count limit (rising)
#define MPR121_FILTER_DELAY_COUNT_RISING (0x2E)  // Filter delay count (rising)
#define MPR121_MAX_HALF_DELTA_FALLING (0x2F)     // Max half delta (falling)
#define MPR121_NOISE_HALF_DELTA_FALLING (0x30)   // Noise half delta (falling)
#define MPR121_NOISE_COUNT_LIMIT_FALLING (0x31)  // Noise count limit (falling)
#define MPR121_FILTER_DELAY_COUNT_FALLING (0x32) // Filter delay count (falling)
// There is no max half delta for touched
#define MPR121_NOISE_HALF_DELTA_TOUCHED (0x33)   // Noise half delta (touched)
#define MPR121_NOISE_COUNT_LIMIT_TOUCHED (0x34)  // Noise count limit (touched)
#define MPR121_FILTER_DELAY_COUNT_TOUCHED (0x35) // Filter delay count (touched)
// (0x41~0x5A) Touch / release threshold
#define MPR121_TOUCH_THRESHOLD (0x41)   // Touch threshold (0th, += 2 for each electrode up to 11th)
#define MPR121_RELEASE_THRESHOLD (0x42) // Release threshold (0th, += 2 for each electrode up to 11th)
#define MPR121_DEBOUNCE (0x5B)          // Debounce
// (0x5C~0x5D) Filter and global CDC CDT configuration
#define MPR121_CONFIG1 (0x5C) // FFI (first filter iterations), CDC (charge/discharge current)
#define MPR121_CONFIG2 (0x5D) // CDT (charge/discharge time), SFI (second filter iterations), ESI (electrode sample interval)
// (0x5F~0x6B) Electrode charge current
// (0x6C~0x72) Electrode charge time
#define MPR121_ELECTRODE_CONFIG (0x5E) // Electrode configuration register
// (0x73~0x7A) GPIO
// (0x73) GPIO control 0
// (0x74) GPIO control 1
// (0x75) GPIO data
// (0x76) GPIO direction
// (0x77) GPIO enable
// (0x78) GPIO data set
// (0x79) GPIO data clear
// (0x7A) GPIO data toggle
// (0x7B) Auto-config control 0
// (0x7C) Auto-config control 1
// (0x7D) Auto-config upper-side limit
// (0x7E) Auto-config lower-side limit
// (0x7F) Auto-config target level
#define MPR121_SOFT_RESET (0x80) // Soft reset

void Mpr121KbI2cImpl::reg8_write(uint8_t addr, uint8_t val)
{
    i2cBus->beginTransmission(MPR121_KB_ADDR);
    i2cBus->write(addr);
    i2cBus->write(val);
    i2cBus->endTransmission();
}

uint8_t Mpr121KbI2cImpl::reg8_read(uint8_t addr)
{
    uint8_t val;
    i2cBus->beginTransmission(MPR121_KB_ADDR);
    i2cBus->write(addr);
    i2cBus->endTransmission();
    i2cBus->requestFrom(MPR121_KB_ADDR, 1);
    val = i2cBus->read();
    i2cBus->endTransmission();
    return val;
}

uint16_t Mpr121KbI2cImpl::reg16_read(uint8_t addr)
{
    uint16_t val;
    uint8_t msb;
    uint8_t lsb;
    i2cBus->beginTransmission(MPR121_KB_ADDR);
    i2cBus->write(addr);
    i2cBus->endTransmission();
    i2cBus->requestFrom(MPR121_KB_ADDR, 2);
    lsb = i2cBus->read();
    msb = i2cBus->read();
    i2cBus->endTransmission();
    val = (msb << 8) | lsb;
    return val;
}

int32_t Mpr121KbI2cImpl::read()
{
    uint16_t new_state = touched();
    uint16_t mask = 1;
    LOG_INFO("Running the MPR121 handler: %u", new_state);
    for (int i = 0; i < 12; i++) {
        if ((old_state & mask) && (!(new_state & mask))) {
            kb12key->key(i);
        }
        mask <<= 1;
    }
    old_state = new_state;
    return INT32_MAX;
}

void Mpr121KbI2cImpl::set_thresholds(uint8_t touch, uint8_t release)
{
    // Sets the touch and release thresholds (0-255) for all electrodes
    //  you can only modify the thresholds when in stop mode
    uint8_t config = reg8_read(MPR121_ELECTRODE_CONFIG);
    if (config != 0) {
        reg8_write(MPR121_ELECTRODE_CONFIG, 0);
    }
    for (int i = 0; i < 12; i++) {
        reg8_write(MPR121_TOUCH_THRESHOLD + i * 2, touch);
        reg8_write(MPR121_RELEASE_THRESHOLD + i * 2, release);
    }
    // return to previous mode if temporarily entered stop mode
    if (config != 0) {
        reg8_write(MPR121_ELECTRODE_CONFIG, config);
    }
}

uint16_t Mpr121KbI2cImpl::filtered_data(int electrode)
{
    // Returns filtered data value for the specified electrode (0-11)
    if ((electrode < 0) || (electrode > 11)) {
        assert(0);
    }
    return reg16_read(MPR121_ELECTRODE_FILTERED_DATA + electrode * 2);
}

uint8_t Mpr121KbI2cImpl::baseline_data(int electrode)
{
    // Returns baseline data value for the specified electrode (0-11)
    if ((electrode < 0) || (electrode > 11)) {
        assert(0);
    }
    return reg8_read(MPR121_BASELINE_VALUE + electrode) << 2;
}

uint16_t Mpr121KbI2cImpl::touched()
{
    // Returns a 12-bit value representing which electrodes are touched. LSB = electrode 0
    return reg16_read(MPR121_TOUCH_STATUS);
}

int Mpr121KbI2cImpl::is_touched(int electrode)
{
    if ((electrode < 0) || (electrode > 11)) {
        assert(0);
    }
    uint16_t t = touched();
    return (t & (1 << electrode)) != 0;
}

void Mpr121KbI2cImpl::reset()
{
    // Resets the MPR121 to a default state
    reg8_write(MPR121_SOFT_RESET, 0x63);
    // Reset electrode configuration to defaults - enter stop mode
    // Config registers are read-only unless in stop mode
    reg8_write(MPR121_ELECTRODE_CONFIG, 0x00);

    // Check CDT, SFI, ESI configuration is at defaults
    // A soft reset puts CONFIG2 (0x5D) at 0x24
    // Charge Discharge Time, CDT=1 (0.5us charge time)
    // Second Filter Iterations, SFI=0 (4x samples taken)
    // Electrode Sample Interval, ESI=4 (16ms period)
    if (reg8_read(MPR121_CONFIG2) != 0x24) {
        // Failed to reset MPR121 to default state
    }
    // Set touch and release trip thresholds
    set_thresholds(15, 7);

    // Configure electrode filtered data and baseline registers
    reg8_write(MPR121_MAX_HALF_DELTA_RISING, 0x01);
    reg8_write(MPR121_MAX_HALF_DELTA_FALLING, 0x01);
    reg8_write(MPR121_NOISE_HALF_DELTA_RISING, 0x01);
    reg8_write(MPR121_NOISE_HALF_DELTA_FALLING, 0x05);
    reg8_write(MPR121_NOISE_HALF_DELTA_TOUCHED, 0x00);
    reg8_write(MPR121_NOISE_COUNT_LIMIT_RISING, 0x0E);
    reg8_write(MPR121_NOISE_COUNT_LIMIT_FALLING, 0x01);
    reg8_write(MPR121_NOISE_COUNT_LIMIT_TOUCHED, 0x00);
    reg8_write(MPR121_FILTER_DELAY_COUNT_RISING, 0x00);
    reg8_write(MPR121_FILTER_DELAY_COUNT_FALLING, 0x00);
    reg8_write(MPR121_FILTER_DELAY_COUNT_TOUCHED, 0x00);

    // Set config registers
    // Debounce Touch, DT=0 (increase up to 7 to reduce noise)
    // Debounce Release, DR=0 (increase up to 7 to reduce noise)
    reg8_write(MPR121_DEBOUNCE, 0x00);
    // First Filter Iterations, FFI=0 (6x samples taken)
    // Charge Discharge Current, CDC=16 (16uA)
    reg8_write(MPR121_CONFIG1, 0x10);
    // Charge Discharge Time, CDT=1 (0.5us charge time)
    // Second Filter Iterations, SFI=0 (4x samples taken)
    // Electrode Sample Interval, ESI=0 (1ms period)
    reg8_write(MPR121_CONFIG2, 0x20);

    // Enable all electrodes - enter run mode
    // Calibration Lock, CL=10 (baseline tracking enabled, initial value 5 high bits)
    // Proximity Enable, ELEPROX_EN=0 (proximity detection disabled)
    // Electrode Enable, ELE_EN=15 (enter run mode for 12 electrodes)
    reg8_write(MPR121_ELECTRODE_CONFIG, 0x8F);
}
