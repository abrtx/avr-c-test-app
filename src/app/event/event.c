#include <util/atomic.h>
#include "event.h"

// -----------------------------
#define EVENT_QUEUE_SIZE 8

static volatile EventType queue[EVENT_QUEUE_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

// -----------------------------
// PUSH (can be called from ISR)
// -----------------------------
void event_push(EventType event) {

    uint8_t next;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        next = (head + 1) % EVENT_QUEUE_SIZE;

        // avoid overflow
        if (next != tail) {
            queue[head] = event;
            head = next;
        }
    }
}
// -----------------------------
// POP (called from main loop)
// -----------------------------
EventType event_pop(void) {

    EventType event = EVENT_NONE;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        if (head != tail) {
            event = queue[tail];
            tail = (tail + 1) % EVENT_QUEUE_SIZE;
        }
    }

    return event;
}
