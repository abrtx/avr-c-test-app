#include "state.h"
#include "led.h"

// external from app
extern volatile uint32_t millis;
extern Led leds[];
extern int LED_COUNT;

static AppState current_state = STATE_OFF;

// -----------------------------
void state_set(AppState new_state) {
    current_state = new_state;
}

AppState state_get(void) {
    return current_state;
}

// -----------------------------
// EVENT TRANSITION
// -----------------------------
void state_handle_event(void) {
    switch (current_state) {
        case STATE_OFF:
            current_state = STATE_BLINK;
            break;

        case STATE_BLINK:
            current_state = STATE_RUNNING;
            break;

        case STATE_RUNNING:
            current_state = STATE_OFF;
            break;
    }
}

// -----------------------------
// STATES
// -----------------------------
static void state_off(void) {
    for (int i = 0; i < LED_COUNT; i++)
        led_off(&leds[i]);
}

static void state_blink(void) {
    static uint32_t last = 0;

    if ((millis - last) > 200) {
        for (int i = 0; i < LED_COUNT; i++)
            led_toggle(&leds[i]);

        last = millis;
    }
}

static void state_running(void) {
    static uint32_t last = 0;
    static int index = 0;

    if ((millis - last) > 100) {

        for (int i = 0; i < LED_COUNT; i++)
            led_off(&leds[i]);

        led_on(&leds[index]);

        index = (index + 1) % LED_COUNT;
        last = millis;
    }
}

// -----------------------------
// MAIN DISPATCH
// -----------------------------
void state_run(void) {
    switch (current_state) {
        case STATE_OFF:
            state_off();
            break;

        case STATE_BLINK:
            state_blink();
            break;

        case STATE_RUNNING:
            state_running();
            break;
    }
}
