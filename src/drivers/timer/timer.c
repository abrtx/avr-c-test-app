#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

// -----------------------------
static volatile uint32_t millis = 0;

// -----------------------------
// TIMER0 ISR (1ms)
// -----------------------------
ISR(TIMER0_COMPA_vect) {
    millis++;
}

// -----------------------------
void timer_init(void) {

    // CTC mode
    TCCR0A |= (1 << WGM01);

    // 16MHz / 64 / 1000 = 249
    OCR0A = 249;

    // prescaler 64
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // enable interrupt
    TIMSK0 |= (1 << OCIE0A);
}

// -----------------------------
uint32_t timer_now(void) {
    return millis;
}

// -----------------------------
uint8_t timer_elapsed(uint32_t last, uint32_t interval) {
    return (uint32_t)(millis - last) >= interval;
}
