#include <string.h>
#include <stdlib.h> // for atoi

#include "cmd.h"
#include "uart.h"
#include "state.h"
#include "led_pattern.h"
#include "log.h"
#include "config.h"
#include "cmd_internal.h"
#include "cmd_registry.h"


#define CMD_BUFFER_SIZE 32
#define MAX_ARGS 8

static char buffer[CMD_BUFFER_SIZE];
static uint8_t buf_index = 0;
static uint8_t initialized = 0;

static void cmd_help(int argc, char **argv); // forward
void cmd_led(int argc, char **argv);

static void cmd_prompt(void) {
    log_fmt("> ");
}


// -----------------------------
// Command handlers
// -----------------------------
static void cmd_help(int argc, char **argv);

const Command cmd_core_commands[] = {
    { "help", cmd_help, "Show this help" },
};

const int cmd_core_count =
    sizeof(cmd_core_commands) / sizeof(Command);

// -----------------------------
// Help command
// -----------------------------
static void cmd_help(int argc, char **argv){

    int count;
    const Command *commands = cmd_get_commands(&count);

    log_fmt("Commands:\r\n");

    for (int i = 0; i < count; i++) {
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

    int count = 0;
    const Command *commands = cmd_get_commands(&count);

    for (int i = 0; i < count; i++) {

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

    if (!initialized) {
        cmd_prompt();     // ✅ first prompt here
        initialized = 1;
    }

    while (uart_available()) {

        char c = uart_read_char();

        // -----------------------------
        // IGNORE ESC SEQUENCES
        // -----------------------------
        if (c == 27) {
            uart_read_char();
            uart_read_char();
            continue;
        }

        // -----------------------------
        // ENTER (ONLY '\r')
        // -----------------------------
        if (c == '\r' || c == '\n') {

            uart_write_string("\r\n");

            buffer[buf_index] = '\0';

            if (buf_index > 0) {
                process_command(buffer);
            }

            buf_index = 0;

            cmd_prompt();   // ✅ ALWAYS print prompt here
            continue;
        }

        // ignore '\n'
        if (c == '\n') {
            continue;
        }

        // -----------------------------
        // BACKSPACE
        // -----------------------------
        if (c == 127 || c == '\b') {

            if (buf_index > 0) {
                buf_index--;
                uart_write_string("\b \b");
            }

            continue;
        }

        // -----------------------------
        // PRINTABLE
        // -----------------------------
        if (c >= 32 && c <= 126) {

            if (buf_index < CMD_BUFFER_SIZE - 1) {

                buffer[buf_index++] = c;
                #ifndef HOST_BUILD
		uart_write_char(c);
                #endif
            }
        }
    }
}


