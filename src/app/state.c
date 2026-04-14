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
	    log_fmt("State -> BLINK\r\n");
            break;

        case STATE_BLINK:
            state_set(STATE_RUNNING);
            led_pattern_set(LED_PATTERN_RUNNING);
	    log_fmt("State -> RUNNING\r\n"); 
            break;

        case STATE_RUNNING:
            state_set(STATE_OFF);
            led_pattern_set(LED_PATTERN_OFF);
	    log_fmt("State -> OFF\r\n");
            break;
    }
}

const char* state_to_string(AppState state) {

    switch (state) {
        case STATE_OFF:     return "OFF";
        case STATE_BLINK:   return "BLINK";
        case STATE_RUNNING: return "RUNNING";
        default:            return "UNKNOWN";
    }
}


// -----------------------------
// MAIN LOOP (logic only)
// -----------------------------
void state_run(void) {
    // intentionally empty for now
}
