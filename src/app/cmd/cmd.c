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
// Command handlers
// -----------------------------
static void cmd_off(void) {
    state_set(STATE_OFF);
    led_pattern_set(LED_PATTERN_OFF);
    log_fmt("OK: OFF\r\n");
}

static void cmd_blink(void) {
    state_set(STATE_BLINK);
    led_pattern_set(LED_PATTERN_BLINK);
    log_fmt("OK: BLINK\r\n");
}

static void cmd_run(void) {
    state_set(STATE_RUNNING);
    led_pattern_set(LED_PATTERN_RUNNING);
    log_fmt("OK: RUNNING\r\n");
}

static void cmd_status(void) {
    log_fmt("State: %s\r\n", state_to_string(state_get()));
    log_fmt("LEDs: %d\r\n", LED_COUNT);
}

static void cmd_help(void); // forward

// -----------------------------
// Command table
// -----------------------------
typedef struct {
    const char *name;
    void (*handler)(void);
    const char *help;
} Command;

static const Command commands[] = {
    { "off",    cmd_off,    "Turn OFF LEDs" },
    { "blink",  cmd_blink,  "Blink LEDs" },
    { "run",    cmd_run,    "Running pattern" },
    { "status", cmd_status, "Show system status" },
    { "help",   cmd_help,   "Show this help" },
};

#define CMD_COUNT (sizeof(commands) / sizeof(commands[0]))

// -----------------------------
// Help command
// -----------------------------
static void cmd_help(void) {

    log_fmt("Commands:\r\n");

    for (uint8_t i = 0; i < CMD_COUNT; i++) {
        log_fmt(" - %s: %s\r\n",
            commands[i].name,
            commands[i].help);
    }
}

// -----------------------------
// Dispatcher
// -----------------------------
static void process_command(const char *cmd) {

    for (uint8_t i = 0; i < CMD_COUNT; i++) {

        if (strcmp(cmd, commands[i].name) == 0) {
            commands[i].handler();
            return;
        }
    }

    log_fmt("ERR: Unknown command\r\n");
}

// -----------------------------
// UART processing
// -----------------------------
void cmd_update(void) {

    while (uart_available()) {

        char c = uart_read_char();

        if (c == '\r' || c == '\n') {

            buffer[index] = '\0';

            if (index > 0) {   // 🔥 avoid empty command
                process_command(buffer);
            }

            index = 0;

        } else if (index < CMD_BUFFER_SIZE - 1) {

            buffer[index++] = c;
        }
    }
}
