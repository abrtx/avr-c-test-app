#include <string.h>

#include "cmd.h"
#include "uart.h"
#include "state.h"
#include "led_pattern.h"
#include "log.h"
#include "config.h"

#define CMD_BUFFER_SIZE 32

static char buffer[CMD_BUFFER_SIZE];
static uint8_t index = 0;

// -----------------------------
static void process_command(const char *cmd) {

    if (strcmp(cmd, "off") == 0) {

        state_set(STATE_OFF);
        led_pattern_set(LED_PATTERN_OFF);
        log_fmt("OK: OFF\r\n");

    } else if (strcmp(cmd, "blink") == 0) {

        state_set(STATE_BLINK);
        led_pattern_set(LED_PATTERN_BLINK);
        log_fmt("OK: BLINK\r\n");

    } else if (strcmp(cmd, "run") == 0) {

        state_set(STATE_RUNNING);
        led_pattern_set(LED_PATTERN_RUNNING);
        log_fmt("OK: RUNNING\r\n");

    } 
    // 🔥 NEW COMMAND
    else if (strcmp(cmd, "status") == 0) {

        log_fmt("State: %s\r\n", state_to_string(state_get()));
        log_fmt("LEDs: %d\r\n", LED_COUNT);

    } 
    else {

        log_fmt("ERR: Unknown command\r\n");
    }
}


// -----------------------------
void cmd_update(void) {

    while (uart_available()) {

        char c = uart_read_char();

        if (c == '\r' || c == '\n') {

            buffer[index] = '\0';
            process_command(buffer);
            index = 0;

        } else if (index < CMD_BUFFER_SIZE - 1) {

            buffer[index++] = c;
        }
    }
}
