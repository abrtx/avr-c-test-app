#include "cmd_registry.h"

// include modules
extern const Command cmd_led_commands[];
extern const int cmd_led_count;

extern const Command cmd_state_commands[];
extern const int cmd_state_count;

extern Command cmd_debug_commands[];
extern const int cmd_debug_count;

// static buffer
static Command all_commands[16];

extern const Command* cmd_get_core_commands(int *count);

extern const Command cmd_core_commands[];
extern const int cmd_core_count;


const Command* cmd_get_commands(int *count) {

    int index = 0;

    // copy LED commands
    for (int i = 0; i < cmd_led_count; i++)
        all_commands[index++] = cmd_led_commands[i];

    // copy state commands
    for (int i = 0; i < cmd_state_count; i++)
        all_commands[index++] = cmd_state_commands[i];

    // copy debug commands
    for (int i = 0; i < cmd_debug_count; i++)
        all_commands[index++] = cmd_debug_commands[i];

    // core commands
    for (int i = 0; i < cmd_core_count; i++)
	all_commands[index++] = cmd_core_commands[i];


    *count = index;
    return all_commands;
}
