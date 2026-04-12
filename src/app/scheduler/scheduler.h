#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

// function pointer type
typedef void (*TaskCallback)(void);

// -----------------------------
typedef struct {
    TaskCallback callback;
    uint32_t interval;
    uint32_t last_run;
} Task;

// -----------------------------
void scheduler_add(Task *task);
void scheduler_run(void);

#endif
