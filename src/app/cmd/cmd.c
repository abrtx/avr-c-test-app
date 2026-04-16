#include <string.h>
#include <stdlib.h> // for atoi

#include "cmd.h"
#include "uart.h"
#include "state.h"
#include "led_pattern.h"
#include "log.h"
#include "config.h"

extern Led leds[];

#define CMD_BUFFER_SIZE 32
#define MAX_ARGS 8

static char buffer[CMD_BUFFER_SIZE];
static uint8_t index = 0;
static uint8_t prompt_shown = 0;

// -----------------------------
// Command handlers
// -----------------------------
static void cmd_off(int argc, char **argv){
    state_set(STATE_OFF);
    led_pattern_set(LED_PATTERN_OFF);
    log_fmt("OK: OFF\r\n");
}

static void cmd_blink(int argc, char **argv) {
    state_set(STATE_BLINK);
    led_pattern_set(LED_PATTERN_BLINK);
    log_fmt("OK: BLINK\r\n");
}

static void cmd_run(int argc, char **argv) {
    state_set(STATE_RUNNING);
    led_pattern_set(LED_PATTERN_RUNNING);
    log_fmt("OK: RUNNING\r\n");
}

static void cmd_status(int argc, char **argv) {
    log_fmt("State: %s\r\n", state_to_string(state_get()));
    log_fmt("LEDs: %d\r\n", LED_COUNT);
}

static void cmd_led(int argc, char **argv) {

    if (argc < 3) {
        log_fmt("Usage: led <index> <on|off|toggle>\r\n");
        return;
    }

    int index = atoi(argv[1]);

    if (index < 0 || index >= LED_COUNT) {
        log_fmt("ERR: Invalid LED index\r\n");
        return;
    }

    // 🔥 disable pattern system
    led_pattern_set(LED_PATTERN_MANUAL);

    if (strcmp(argv[2], "on") == 0) {

        led_on(&leds[index]);
        log_fmt("LED %d ON\r\n", index);

    } else if (strcmp(argv[2], "off") == 0) {

        led_off(&leds[index]);
        log_fmt("LED %d OFF\r\n", index);

    } else if (strcmp(argv[2], "toggle") == 0) {

        led_toggle(&leds[index]);
        log_fmt("LED %d TOGGLE\r\n", index);

    } else {

        log_fmt("ERR: Unknown action\r\n");
    }
}

static void cmd_help(int argc, char **argv); // forward


static void cmd_prompt(void) {
    log_fmt("> ");
}

// -----------------------------
// Command table
// -----------------------------
typedef struct {
    const char *name;
    void (*handler)(int argc, char **argv);
    const char *help;
} Command;

static void cmd_led(int argc, char **argv);


static const Command commands[] = {
    { "off",    cmd_off,    "Turn OFF LEDs" },
    { "blink",  cmd_blink,  "Blink LEDs" },
    { "run",    cmd_run,    "Running pattern" },
    { "status", cmd_status, "Show system status" },
    { "led", cmd_led, "Control LED: led <i> <on|off|toggle>" },
    { "help",   cmd_help,   "Show this help" },
};

#define CMD_COUNT (sizeof(commands) / sizeof(commands[0]))

// -----------------------------
// Help command
// -----------------------------
static void cmd_help(int argc, char **argv){

    log_fmt("Commands:\r\n");

    for (uint8_t i = 0; i < CMD_COUNT; i++) {
        log_fmt(" - %s: %s\r\n",
            commands[i].name,
            commands[i].help);
    }
}




static int parse_args(char *input, char **argv) {

    int argc = 0;

    while (*input && argc < MAX_ARGS) {

        // skip spaces
        while (*input == ' ') input++;

        if (*input == '\0')
            break;

        argv[argc++] = input;

        // find end of word
        while (*input && *input != ' ')
            input++;

        if (*input) {
            *input = '\0'; // terminate string
            input++;
        }
    }

    return argc;
}


// -----------------------------
// Dispatcher
// -----------------------------
static void process_command(char *cmd) {

    char *argv[MAX_ARGS];
    int argc = parse_args(cmd, argv);

    if (argc == 0)
        return;

    for (uint8_t i = 0; i < CMD_COUNT; i++) {

        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].handler(argc, argv);
            return;
        }
    }

    log_fmt("ERR: Unknown command\r\n");
}


// -----------------------------
// UART processing
// -----------------------------
void cmd_update(void) {

    if (!prompt_shown) {
        cmd_prompt();
        prompt_shown = 1;
    }

    while (uart_available()) {

        char c = uart_read_char();

        // -----------------------------
        // IGNORE ESC SEQUENCES (arrows, etc)
        // -----------------------------
        if (c == 27) { // ESC
            uart_read_char(); // skip '['
            uart_read_char(); // skip next
            continue;
        }

        // -----------------------------
        // ENTER (handle ONLY '\r')
        // -----------------------------
        if (c == '\r') {

            uart_write_string("\r\n");

            buffer[index] = '\0';

            if (index > 0) {
                process_command(buffer);
            }

            index = 0;

            cmd_prompt();
            continue;
        }

        // ignore '\n' completely (CRLF fix)
        if (c == '\n') {
            continue;
        }

        // -----------------------------
        // BACKSPACE
        // -----------------------------
        if (c == 127 || c == '\b') {

            if (index > 0) {
                index--;

                // erase visually
                uart_write_string("\b \b");
            }

            continue;
        }

        // -----------------------------
        // PRINTABLE CHARACTERS ONLY
        // -----------------------------
        if (c >= 32 && c <= 126) {

            if (index < CMD_BUFFER_SIZE - 1) {

                buffer[index++] = c;

                // echo clean
                uart_write_char(c);
            }
        }
    }
}




