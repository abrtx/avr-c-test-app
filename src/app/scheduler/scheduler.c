#include "scheduler.h"
#include "timer.h"

#define MAX_TASKS 8

static Task *tasks[MAX_TASKS];
static uint8_t task_count = 0;

// -----------------------------
void scheduler_add(Task *task) {

    if (task_count < MAX_TASKS) {
        tasks[task_count++] = task;
    }
}

// -----------------------------
void scheduler_run(void) {

    uint32_t now = timer_now();

    for (uint8_t i = 0; i < task_count; i++) {

        Task *t = tasks[i];

        if ((now - t->last_run) >= t->interval) {
            t->last_run = now;
            t->callback();
        }
    }
}
