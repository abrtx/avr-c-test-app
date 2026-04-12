#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// initialize timer (1ms tick)
void timer_init(void);

// get current time in ms
uint32_t timer_now(void);

// check if interval elapsed
uint8_t timer_elapsed(uint32_t last, uint32_t interval);

#endif
