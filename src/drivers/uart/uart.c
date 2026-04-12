#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

#define F_CPU 16000000UL

#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 64
//-----------------------------
//TX
static volatile char tx_buffer[UART_TX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;
//RX
static volatile char rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;


// -----------------------------
void uart_init(uint32_t baud) {

    uint16_t ubrr = (F_CPU / 16 / baud) - 1;

    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // enable RX + TX + RX interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // 8N1
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// -----------------------------
static void uart_start_tx(void) {
    // enable UDRE interrupt
    UCSR0B |= (1 << UDRIE0);
}

// -----------------------------
void uart_write_char(char c) {

    uint8_t next = (tx_head + 1) % UART_TX_BUFFER_SIZE;

    // wait if buffer full (rare, short)
    while (next == tx_tail);

    tx_buffer[tx_head] = c;
    tx_head = next;

    uart_start_tx();
}

// -----------------------------
void uart_write_string(const char *str) {

    while (*str) {
        uart_write_char(*str++);
    }
}

uint8_t uart_available(void) {
    return (rx_head != rx_tail);
}

char uart_read_char(void) {

    if (rx_head == rx_tail)
        return 0;

    char c = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % UART_RX_BUFFER_SIZE;

    return c;
}


// -----------------------------
// ISR: send next byte
// -----------------------------
ISR(USART_UDRE_vect) {

    if (tx_head == tx_tail) {
        // buffer empty → disable interrupt
        UCSR0B &= ~(1 << UDRIE0);
    } else {
        UDR0 = tx_buffer[tx_tail];
        tx_tail = (tx_tail + 1) % UART_TX_BUFFER_SIZE;
    }
}

ISR(USART_RX_vect) {

    char c = UDR0;

    uint8_t next = (rx_head + 1) % UART_RX_BUFFER_SIZE;

    if (next != rx_tail) {
        rx_buffer[rx_head] = c;
        rx_head = next;
    }
}

