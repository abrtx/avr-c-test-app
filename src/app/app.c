#include <avr/io.h>
#include <avr/interrupt.h>

#include "app.h"
#include "state.h"
#include "gpio.h"
#include "led.h"
#include "button.h"
#include "event.h"

// -----------------------------
Led leds[] = {
    { &GPIOB, 2 },
    { &GPIOB, 3 },
    { &GPIOB, 4 }
};

int LED_COUNT = 3;

// -----------------------------
volatile uint32_t millis = 0;

// -----------------------------
// TIMER ISR
// -----------------------------
ISR(TIMER0_COMPA_vect) {
    millis++;
}

// -----------------------------
static void timer0_init(void) {
    TCCR0A |= (1 << WGM01);
    OCR0A = 249;
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
}

// -----------------------------
void app_init(void) {

    for (int i = 0; i < LED_COUNT; i++) {
        led_init(&leds[i]);
    }

    button_init();
    timer0_init();

    sei();
}

// -----------------------------
void app_loop(void) {

    // update inputs
    button_update();

    // process events
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

    // run state machine
    state_run();
}

