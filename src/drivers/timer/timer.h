#pragma once
#include <stdint.h>

// in hertz
#define TIMER_FREQ  100
// in ms
#define TIMER_SLICE (1000 / TIMER_FREQ)

// elapsed time in ms
extern uint64_t g_timer_elapsed;

void timer_set_freq(int freq);
void timer_init(void);
void timer_handler(void);