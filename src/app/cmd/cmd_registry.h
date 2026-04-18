#ifndef CMD_REGISTRY_H
#define CMD_REGISTRY_H

typedef struct {
    const char *name;
    void (*handler)(int argc, char **argv);
    const char *help;
} Command;

// each module exposes its commands
const Command* cmd_get_commands(int *count);

#endif
