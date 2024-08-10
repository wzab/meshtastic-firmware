/*
  Written by Wojciech M. Zabolotny (wzab01<at>gmail.com)
  License: MIT
*/
#include "kb12key.h"

Kb12key::Kb12key()
{
    mode = 0;
    pos = 0;
    last_key = 0;
    last_key_timeout = 0;
    queue_head = 0;
    queue_tail = 0;
}

#define KB12KEY_NKEY (12)
#define KB12KEY_NMODES (4)
#define KB12KEY_TIMEOUT (2000)

// Remaping of keys
static uint8_t key_remap[12] = {2, 5, 8, 11, 1, 4, 7, 10, 0, 3, 6, 9};

// Definition of the keyboard layout
static char *kb_layout[KB12KEY_NMODES][KB12KEY_NKEY] = {
    // Mode 0
    {
        " ,.!?", // Key 0
        "abc",   // Key 1
        "def",   // Key 2
        "ghi",   // Key 3
        "jkl",   // Key 4
        "mno",   // Key 5
        "pqr",   // Key 6
        "stuv",  // Key 7
        "wxyz",  // Key 8
        "\x01",  // Key 9 - Mode switch
        "\x08",  // Key 10 - Backspace
        "\x0d",  // Key 11 - Enter
    },
    // Mode 1
    {
        " ,.!?", // Key 0
        "ABC",   // Key 1
        "DEF",   // Key 2
        "GHI",   // Key 3
        "JKL",   // Key 4
        "MNO",   // Key 5
        "PQR",   // Key 6
        "STUV",  // Key 7
        "WXYZ",  // Key 8
        "\x01",  // Key 9 - Mode switch
        "\x08",  // Key 10 - Backspace
        "\x0d",  // Key 11 - Enter
    },
    // Mode 2
    {
        "1(",   // Key 0
        "2)",   // Key 1
        "3{",   // Key 2
        "4}",   // Key 3
        "5+",   // Key 4
        "6-",   // Key 5
        "7/",   // Key 6
        "8_",   // Key 7
        "9%",   // Key 8
        "\x01", // Key 9 - Mode switch
        "0",    // Key 10 - Zero in that mode
        "\x0d", // Key 11 - Enter
    },
    // Mode 3
    {
        "?",    // Key 0
        "\xb5", // Key 1 - Arrow UP
        "?",    // Key 2
        "\xb4", // Key 3 - Arrow LEFT
        "?",    // Key 4
        "\xb7", // Key 5 - Arrow RIGHT
        "\x09", // Key 6 - TAB
        "\xb6", // Key 7 - Arrow DOWN
        "?",    // Key 8
        "\x01", // Key 9 - Mode switch
        "\x08", // Key 10 - Backspace
        "\x0d", // Key 11 - Enter
    }};

void Kb12key::emit(uint8_t key)
{
    uint8_t new_head = (queue_head + 1) % QUEUE_LENGTH;
    if (new_head != queue_tail) {
        queue[queue_head] = key;
        queue_head = new_head;
    }
}

uint8_t Kb12key::empty()
{
    return queue_tail == queue_head ? 1 : 0;
}

uint8_t Kb12key::get()
{
    if (queue_tail == queue_head)
        return 0;
    uint8_t res = queue[queue_head];
    queue_tail = (queue_tail + 1) % QUEUE_LENGTH;
    return res;
}

void Kb12key::tick()
{
    if (last_key_timeout) {
        last_key_timeout--;
    }
}

void Kb12key::key(int key)
{
    if ((key < 0) || (key > 11)) {
        return;
    }
    // Remap key
    key = key_remap[key];
    // Certain keys are treated in a special way
    if (key == 9) {
        // This is a mode switch
        mode = (mode + 1) % KB12KEY_NMODES;
        last_key_timeout = 0;
        // Set the LED blinking to reflect the mode
        // TBD!
        return;
    }
    if (key == 11) {
        // This is an ENTER switch
        mode = 0;
        last_key_timeout = 0;
        // Set the LED blinking to reflect the mode
        // TBD!
        emit(0x0d);
        return;
    }
    // Check if that's a repeated key press (except mode 3!)
    if ((key == last_key) && (last_key_timeout) && (mode != 3)) {
        pos++;
        if (kb_layout[mode][key][pos] == 0) {
            // End of list reached
            pos = 0;
        }
        // Repeated key press
        emit(0x08); // backspace
        emit(kb_layout[mode][key][pos]);
    } else {
        // The key was pressed for the first time
        emit(kb_layout[mode][key][0]);
        pos = 0;
        last_key = key;
        last_key_timeout = KB12KEY_TIMEOUT;
    }
}
