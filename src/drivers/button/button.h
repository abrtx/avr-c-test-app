#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "gpio.h"

// -----------------------------
typedef enum {
    BUTTON_ACTIVE_LOW = 0,
    BUTTON_ACTIVE_HIGH
} ButtonMode;

// -----------------------------
typedef struct {
    Gpio *gpio;
    uint8_t pin;
    ButtonMode mode;

    // internal state
    uint8_t last_state;
    uint32_t last_time;
    uint8_t pressed_flag;

} Button;

// -----------------------------
void button_init(Button *btn);
void button_update(Button *btn);
uint8_t button_pressed(Button *btn);

#endif
