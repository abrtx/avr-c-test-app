#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

// -----------------------------
// Event types
// -----------------------------
typedef enum {
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESSED
} EventType;

// -----------------------------
void event_push(EventType event);
EventType event_pop(void);

#endif
