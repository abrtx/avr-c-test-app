#include <avr/io.h>
#include <avr/interrupt.h>

#include "button.h"
#include "event.h"

// shared time from app
extern volatile uint32_t millis;

// -----------------------------
#define BUTTON_PIN PD2
#define DEBOUNCE_MS 50

// -----------------------------
static volatile uint8_t raw_event = 0;
static uint8_t pressed_flag = 0;
static uint32_t last_time = 0;

// -----------------------------
// ISR: ONLY signal
// -----------------------------
ISR(INT0_vect) {
    raw_event = 1;
}

// -----------------------------
void button_init(void) {

    // PD2 input
    DDRD &= ~(1 << BUTTON_PIN);

    // pull-up
    PORTD |= (1 << BUTTON_PIN);

    // falling edge
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    // enable interrupt
    EIMSK |= (1 << INT0);
}

// -----------------------------
// Non-blocking debounce
// -----------------------------
void button_update(void) {

    if (raw_event) {

        uint32_t now = millis;

        if ((now - last_time) > DEBOUNCE_MS) {

            if (!(PIND & (1 << BUTTON_PIN))) {
                event_push(EVENT_BUTTON_PRESSED);
                last_time = now;
            }
        }

        raw_event = 0;
    }
}

// -----------------------------
// Public API
// -----------------------------
uint8_t button_pressed(void) {
    if (pressed_flag) {
        pressed_flag = 0;
        return 1;
    }
    return 0;
}
