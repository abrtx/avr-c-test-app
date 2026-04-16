#ifndef LED_PATTERN_H
#define LED_PATTERN_H

#include <stdint.h>
#include "led.h"

// -----------------------------
typedef enum {
    LED_PATTERN_OFF = 0,
    LED_PATTERN_BLINK,
    LED_PATTERN_RUNNING,
    LED_PATTERN_MANUAL
} LedPattern;

// -----------------------------
void led_pattern_init(Led *leds, int count);
void led_pattern_set(LedPattern pattern);
void led_pattern_update(void);

#endif
