#ifndef CMD_INTERNAL_H
#define CMD_INTERNAL_H

typedef void (*CmdHandler)(int argc, char **argv);

typedef struct {
    const char *name;
    CmdHandler handler;
    const char *help;
} Command;

#endif
