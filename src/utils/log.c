#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "uart.h"

// -----------------------------
#define LOG_BUFFER_SIZE 64

// -----------------------------
void log_init(void) {
    uart_init(9600);
}

// -----------------------------
void log_write(const char *msg) {
    uart_write_string(msg);
}

// -----------------------------
void log_fmt(const char *fmt, ...) {

    char buffer[LOG_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, LOG_BUFFER_SIZE, fmt, args);

    va_end(args);

    uart_write_string(buffer);
}
