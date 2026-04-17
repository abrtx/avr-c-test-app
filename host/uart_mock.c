#include <stdio.h>

int uart_available(void) {
    return 1;
}

char uart_read_char(void) {
    return getchar();
}

void uart_write_char(char c) {
    putchar(c);
}

void uart_write_string(const char *s) {
    while (*s) putchar(*s++);
}
