#include <avr/io.h>
#include <avr/interrupt.h>

#include "app.h"
#include "state.h"
#include "gpio.h"
#include "led.h"
#include "button.h"
#include "event.h"
#include "timer.h"

// -----------------------------
// LEDs
// -----------------------------
Led leds[] = {
    { &GPIOB, 2 },
    { &GPIOB, 3 },
    { &GPIOB, 4 }
};

int LED_COUNT = 3;

// -----------------------------
// BUTTONS
// -----------------------------
Button btn1 = { &GPIOD, 2, BUTTON_ACTIVE_LOW };

// -----------------------------
void app_init(void) {

    for (int i = 0; i < LED_COUNT; i++) {
        led_init(&leds[i]);
    }

    button_init(&btn1);
    timer_init();

    sei();
}

// -----------------------------
void app_loop(void) {

    button_update(&btn1);

    if (button_pressed(&btn1)) {
        event_push(EVENT_BUTTON_PRESSED);
    }

    EventType ev;

    while ((ev = event_pop()) != EVENT_NONE) {

        switch (ev) {

            case EVENT_BUTTON_PRESSED:
                state_handle_event();
                break;

            default:
                break;
        }
    }

    state_run();
}
