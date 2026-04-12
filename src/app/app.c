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
#include "led_pattern.h"
#include "config.h"
#include "log.h"
#include "cmd.h"

// -----------------------------
// LEDs
// -----------------------------
Led leds[] = {
    { &LED1_GPIO, LED1_PIN },
    { &LED2_GPIO, LED2_PIN },
    { &LED3_GPIO, LED3_PIN }
};


// -----------------------------
// BUTTON
// -----------------------------
Button btn1 = { &BUTTON1_GPIO, BUTTON1_PIN, BUTTON1_MODE };

// -----------------------------
// TASK WRAPPERS
// -----------------------------
void task_button(void) {
    button_update(&btn1);

    if (button_pressed(&btn1)) {
        event_push(EVENT_BUTTON_PRESSED);
    }
}

void task_cmd(void) {
    cmd_update();
}

void task_events(void) {

    EventType ev;

    while ((ev = event_pop()) != EVENT_NONE) {

        switch (ev) {
            case EVENT_BUTTON_PRESSED:
		log_write("Button pressed\n");
                state_handle_event();
                break;

            default:
                break;
        }
    }
}

void task_led(void) {
    led_pattern_update();
}


// -----------------------------
// TASKS
// -----------------------------
Task t_button = { task_button, 10, 0 }; // every 10 ms
Task t_events = { task_events, 0, 0 };  // every loop
Task t_led = { task_led, 10, 0 }; // every 10 ms
Task t_cmd = { task_cmd, 10, 0 };

// Task t_state  = { task_state, 0, 0 };   // every loop

// -----------------------------
void app_init(void) {

    for (int i = 0; i < LED_COUNT; i++) {
        led_init(&leds[i]);
    }

    button_init(&btn1);
    timer_init();
    led_pattern_init(leds, LED_COUNT);
    log_init();


    // register tasks
    scheduler_add(&t_button);
    scheduler_add(&t_events);
    scheduler_add(&t_led);     // 🔥 MISSING BEFORE
    scheduler_add(&t_cmd);
    //scheduler_add(&t_state);

    sei();
}


// -----------------------------
void app_loop(void) {
    scheduler_run();
}
