#include "cmd_registry.h"
#include "log.h"

const Command cmd_debug_commands[];
const int cmd_debug_count;


// -----------------------------
static void cmd_debug(int argc, char **argv) {
    log_fmt("DEBUG OK\r\n");
}

// -----------------------------
const Command cmd_debug_commands[] = {
    { "debug", cmd_debug, "Debug command" },
};

const int cmd_debug_count =
    sizeof(cmd_debug_commands) / sizeof(Command);
