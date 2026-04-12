#include <avr/io.h>
#include <avr/interrupt.h>

#include "button.h"
#include "event.h"
#include "timer.h"

#define BUTTON_PIN PD2
#define DEBOUNCE_MS 50

static volatile uint8_t raw_event = 0;
static uint32_t last_time = 0;

ISR(INT0_vect) {
    raw_event = 1;
}

void button_init(void) {

    DDRD &= ~(1 << BUTTON_PIN);
    PORTD |= (1 << BUTTON_PIN);

    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0);
}

void button_update(void) {

    if (raw_event) {

        uint32_t now = timer_now();

        if ((now - last_time) > DEBOUNCE_MS) {

            if (!(PIND & (1 << BUTTON_PIN))) {
                event_push(EVENT_BUTTON_PRESSED);
                last_time = now;
            }
        }

        raw_event = 0;
    }
}

uint8_t button_pressed(void) {
    return 0;
}
