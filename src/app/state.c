#include "state.h"
#include "led_pattern.h"
#include "log.h"
// -----------------------------
static AppState current_state = STATE_OFF;

// -----------------------------
void state_set(AppState new_state) {
    current_state = new_state;
}

AppState state_get(void) {
    return current_state;
}

// -----------------------------
// EVENT TRANSITION
// -----------------------------
void state_handle_event(void) {

    switch (current_state) {

        case STATE_OFF:
            state_set(STATE_BLINK);
            led_pattern_set(LED_PATTERN_BLINK);
	    log_fmt("State -> BLINK\n");
            break;

        case STATE_BLINK:
            state_set(STATE_RUNNING);
            led_pattern_set(LED_PATTERN_RUNNING);
	    log_fmt("State -> RUNNING\n"); 
            break;

        case STATE_RUNNING:
            state_set(STATE_OFF);
            led_pattern_set(LED_PATTERN_OFF);
	    log_fmt("State -> OFF\n");
            break;
    }
}

// -----------------------------
// MAIN LOOP (logic only)
// -----------------------------
void state_run(void) {
    // intentionally empty for now
}
