#include <string.h>
#include <stdlib.h>

#include "cmd_internal.h"
#include "log.h"
#include "led.h"
#include "led_pattern.h"
#include "config.h"


extern Led leds[];

// -----------------------------
// Subcommands
// -----------------------------
static void led_on_cmd(int argc, char **argv) {

    if (argc < 2) {
        log_fmt("Usage: led on <index>\r\n");
        return;
    }

    int i = atoi(argv[1]);

    if (i < 0 || i >= LED_COUNT) {
        log_fmt("Invalid index\r\n");
        return;
    }

    led_pattern_set(LED_PATTERN_MANUAL);
    led_on(&leds[i]);

    log_fmt("LED %d ON\r\n", i);
}

static void led_off_cmd(int argc, char **argv) {

    if (argc < 2) {
        log_fmt("Usage: led off <index>\r\n");
        return;
    }

    int i = atoi(argv[1]);

    led_pattern_set(LED_PATTERN_MANUAL);
    led_off(&leds[i]);

    log_fmt("LED %d OFF\r\n", i);
}

static void led_toggle_cmd(int argc, char **argv) {

    if (argc < 2) {
        log_fmt("Usage: led toggle <index>\r\n");
        return;
    }

    int i = atoi(argv[1]);

    led_pattern_set(LED_PATTERN_MANUAL);
    led_toggle(&leds[i]);

    log_fmt("LED %d TOGGLE\r\n", i);
}

// -----------------------------
// Subcommand table
// -----------------------------
static const Command subcommands[] = {
    { "on",     led_on_cmd,     "Turn ON LED" },
    { "off",    led_off_cmd,    "Turn OFF LED" },
    { "toggle", led_toggle_cmd, "Toggle LED" },
};

#define SUB_COUNT (sizeof(subcommands) / sizeof(subcommands[0]))

// -----------------------------
// Entry point (called from cmd.c)
// -----------------------------
void cmd_led(int argc, char **argv) {

    if (argc < 2) {
        log_fmt("Usage: led <on|off|toggle> <index>\r\n");
        return;
    }

    for (int i = 0; i < SUB_COUNT; i++) {

        if (strcmp(argv[1], subcommands[i].name) == 0) {

            subcommands[i].handler(argc - 1, &argv[1]);
            return;
        }
    }

    log_fmt("ERR: Unknown subcommand\r\n");
}
