#include <avr/io.h>
#include <avr/interrupt.h>

#include "app.h"
#include "state.h"
#include "gpio.h"
#include "led.h"
#include "button.h"
#include "event.h"
#include "timer.h"
#include "scheduler.h"

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
// BUTTON
// -----------------------------
Button btn1 = { &GPIOD, 2, BUTTON_ACTIVE_LOW };

// -----------------------------
// TASK WRAPPERS
// -----------------------------
void task_button(void) {
    button_update(&btn1);

    if (button_pressed(&btn1)) {
        event_push(EVENT_BUTTON_PRESSED);
    }
}

void task_events(void) {

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
}

void task_state(void) {
    state_run();
}

// -----------------------------
// TASKS
// -----------------------------
Task t_button = { task_button, 10, 0 }; // every 10 ms
Task t_events = { task_events, 0, 0 };  // every loop
Task t_state  = { task_state, 0, 0 };   // every loop

// -----------------------------
void app_init(void) {

    for (int i = 0; i < LED_COUNT; i++) {
        led_init(&leds[i]);
    }

    button_init(&btn1);
    timer_init();

    // register tasks
    scheduler_add(&t_button);
    scheduler_add(&t_events);
    scheduler_add(&t_state);

    sei();
}

// -----------------------------
void app_loop(void) {
    scheduler_run();
}
