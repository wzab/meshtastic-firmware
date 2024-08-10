#pragma once
#include "InputBroker.h"
#include "Wire.h"
#include "concurrency/OSThread.h"
#include <stdint.h>

#define QUEUE_LENGTH (32)
class Kb12key
{
  public:
    int mode;
    int pos;
    int last_key;
    int last_key_timeout;
    uint8_t queue[QUEUE_LENGTH];
    uint8_t queue_head;
    uint8_t queue_tail;

    Kb12key();
    uint8_t empty();
    void emit(uint8_t key);
    void tick();
    void key(int key);
    uint8_t get();
};
