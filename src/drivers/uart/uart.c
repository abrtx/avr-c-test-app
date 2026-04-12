#include <avr/io.h>

#include "uart.h"

#define F_CPU 16000000UL

// -----------------------------
void uart_init(uint32_t baud) {

    uint16_t ubrr = (F_CPU / 16 / baud) - 1;

    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // enable TX
    UCSR0B = (1 << TXEN0);

    // 8N1
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// -----------------------------
void uart_write_char(char c) {

    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = c;
}

// -----------------------------
void uart_write_string(const char *str) {

    while (*str) {
        uart_write_char(*str++);
    }
}
