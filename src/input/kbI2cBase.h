#pragma once

#include "BBQ10Keyboard.h"
#include "InputBroker.h"
#include "Wire.h"
#include "concurrency/OSThread.h"

class KbI2cBase : public Observable<const InputEvent *>, public concurrency::OSThread
{
  public:
    explicit KbI2cBase(const char *name);
    friend class Mpr121KbI2cImpl;

  protected:
    virtual int32_t runOnce() override;
    virtual uint8_t get() { return 0; };

  private:
    const char *_originName;

    TwoWire *i2cBus = 0;

    BBQ10Keyboard Q10keyboard;
    bool is_sym = false;
};
