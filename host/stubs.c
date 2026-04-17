#include <stdio.h>
#include "state.h"
#include "led_pattern.h"
#include "led.h"

// -----------------------------
// STATE
// -----------------------------
void state_set(AppState s) {
    printf("[STATE SET]\n");
}

AppState state_get(void) {
    return STATE_OFF;
}

const char* state_to_string(AppState s) {
    return "OFF";
}

// -----------------------------
// LED PATTERN
// -----------------------------
void led_pattern_set(LedPattern pattern) {
    printf("[PATTERN SET]\n");
}

void uart_init(void) {
    // do nothing (mock)
}


// -----------------------------
int LED_COUNT = 3;

// Fake LED storage
Led leds[3];

// -----------------------------
void led_on(Led *l) {
    printf("[LED ON]\n");
}

void led_off(Led *l) {
    printf("[LED OFF]\n");
}

void led_toggle(Led *l) {
    printf("[LED TOGGLE]\n");
}
