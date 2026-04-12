#include "log.h"
#include "uart.h"

// -----------------------------
void log_init(void) {
    uart_init(9600);
}

// -----------------------------
void log_write(const char *msg) {
    uart_write_string(msg);
}
