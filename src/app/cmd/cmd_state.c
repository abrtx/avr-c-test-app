#include "cmd_registry.h"
#include "state.h"
#include "led_pattern.h"
#include "log.h"
#include "config.h"

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

// -----------------------------
const Command cmd_state_commands[] = {
    { "off", cmd_off, "Turn OFF LEDs" },
    { "blink", cmd_blink, "Blink LEDs" },
    { "run", cmd_run, "Running pattern" },
    { "status", cmd_status, "Show system status" },
};

const int cmd_state_count =
    sizeof(cmd_state_commands) / sizeof(Command);
