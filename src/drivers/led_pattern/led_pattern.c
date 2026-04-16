#include "led_pattern.h"
#include "timer.h"

// -----------------------------
static Led *led_array;
static int led_count;

static LedPattern current_pattern = LED_PATTERN_OFF;

static uint32_t last = 0;
static int index = 0;

// -----------------------------
void led_pattern_init(Led *leds, int count) {
    led_array = leds;
    led_count = count;
}

// -----------------------------
void led_pattern_set(LedPattern pattern) {

    current_pattern = pattern;

    // reset state
    last = 0;
    index = 0;
}

// -----------------------------
static void pattern_off(void) {
    for (int i = 0; i < led_count; i++)
        led_off(&led_array[i]);
}

// -----------------------------
static void pattern_blink(void) {

    if (timer_elapsed(last, 200)) {

        for (int i = 0; i < led_count; i++)
            led_toggle(&led_array[i]);

        last = timer_now();
    }
}

// -----------------------------
static void pattern_running(void) {

    if (timer_elapsed(last, 100)) {

        for (int i = 0; i < led_count; i++)
            led_off(&led_array[i]);

        led_on(&led_array[index]);

        index = (index + 1) % led_count;
        last = timer_now();
    }
}

// -----------------------------
void led_pattern_update(void) {

    if (current_pattern == LED_PATTERN_MANUAL) {
        return; // 🔥 do nothing
    }
    
    switch (current_pattern) {

        case LED_PATTERN_OFF:
            pattern_off();
            break;

        case LED_PATTERN_BLINK:
            pattern_blink();
            break;

        case LED_PATTERN_RUNNING:
            pattern_running();
            break;

        default:
            break;
    }
}
