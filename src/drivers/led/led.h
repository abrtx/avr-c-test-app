#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "gpio.h"

typedef struct {
    Gpio *gpio;
    uint8_t pin;
} Led;

// API
void led_init(Led *led);
void led_on(Led *led);
void led_off(Led *led);
void led_toggle(Led *led);

#endif
