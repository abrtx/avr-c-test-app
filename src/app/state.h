#ifndef STATE_H
#define STATE_H

#include <stdint.h>

typedef enum {
    STATE_OFF = 0,
    STATE_BLINK,
    STATE_RUNNING
} AppState;

void state_set(AppState new_state);
AppState state_get(void);

void state_handle_event(void);
void state_run(void);
const char* state_to_string(AppState state);


#endif
