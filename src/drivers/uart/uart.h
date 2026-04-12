#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint32_t baud);

// TX
void uart_write_char(char c);
void uart_write_string(const char *str);

// RX
uint8_t uart_available(void);
char uart_read_char(void);

#endif
