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
Led leds[] = {
    { &GPIOB, 2 },
    { &GPIOB, 3 },
    { &GPIOB, 4 }
};

int LED_COUNT = 3;

// -----------------------------
void app_init(void) {

    for (int i = 0; i < LED_COUNT; i++) {
        led_init(&leds[i]);
    }

    button_init();
    timer_init();   // 🔥 moved here

    sei();
}

// -----------------------------
void app_loop(void) {

    button_update();

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
