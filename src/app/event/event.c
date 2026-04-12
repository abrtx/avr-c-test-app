#include "event.h"

// -----------------------------
#define EVENT_QUEUE_SIZE 8

static EventType queue[EVENT_QUEUE_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

// -----------------------------
void event_push(EventType event) {

    uint8_t next = (head + 1) % EVENT_QUEUE_SIZE;

    // avoid overflow (simple strategy)
    if (next != tail) {
        queue[head] = event;
        head = next;
    }
}

// -----------------------------
EventType event_pop(void) {

    if (head == tail) {
        return EVENT_NONE; // empty
    }

    EventType event = queue[tail];
    tail = (tail + 1) % EVENT_QUEUE_SIZE;

    return event;
}
