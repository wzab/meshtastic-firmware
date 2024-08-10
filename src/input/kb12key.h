#pragma once
#include "InputBroker.h"
#include "Wire.h"
#include "concurrency/OSThread.h"
#include "concurrency/Periodic.h"
#include <stdint.h>

#define TICK_PERIOD (100)
#define KB12KEY_TIMEOUT (2000 / TICK_PERIOD)

#define QUEUE_LENGTH (32)
class Kb12key
{
  public:
    concurrency::Periodic *led_and_timeout;
    int led_step;
    int led_pin;

    volatile int mode;
    int pos;
    int last_key;
    volatile int last_key_timeout;
    volatile uint8_t queue[QUEUE_LENGTH];
    volatile uint8_t queue_head;
    volatile uint8_t queue_tail;

    Kb12key(int led_pin);
    uint8_t empty();
    void emit(uint8_t key);
    int32_t tick();
    void key(int key);
    uint8_t get();
};
