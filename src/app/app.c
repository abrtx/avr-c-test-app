#include <avr/io.h>
#include <avr/interrupt.h>

#include "app.h"
#include "state.h"
#include "gpio.h"
#include "led.h"

// -----------------------------
// LED CONFIG
// -----------------------------
Led leds[] = {
    { &GPIOB, 2 },
    { &GPIOB, 3 },
    { &GPIOB, 4 }
};

int LED_COUNT = 3;

// -----------------------------
volatile uint32_t millis = 0;
static volatile uint8_t button_event = 0;

// -----------------------------
// TIMER ISR
// -----------------------------
ISR(TIMER0_COMPA_vect) {
    millis++;
}

// -----------------------------
// BUTTON ISR
// -----------------------------
ISR(INT0_vect) {
    button_event = 1;
}

// -----------------------------
static void timer0_init(void) {
    TCCR0A |= (1 << WGM01);
    OCR0A = 249;
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
}

static void button_init(void) {
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0);
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

    if (button_event) {
        state_handle_event();
        button_event = 0;
    }

    state_run();
}
