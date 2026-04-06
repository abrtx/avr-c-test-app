#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

// Initialize button (INT0 - PD2)
void button_init(void);

// Call periodically (from main loop)
void button_update(void);

// Returns 1 only once per press (debounced)
uint8_t button_pressed(void);

#endif
