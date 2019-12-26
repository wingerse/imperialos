#include "timer.h"
#include "../../x86.h"
#include "../../print.h"

// freq in hertz
#define CHIP_FREQ 1193182

#define CONTROL02 0x43
#define CONTROL3  0x47
#define COUNT0    0x40
#define COUNT2    0x42
#define COUNT3    0x44

uint64_t g_timer_elapsed = 0;

void timer_set_freq(int freq)
{
    int divisor = CHIP_FREQ / freq;
    // counter 0, read/write lsb, msb, square wave, 16 bit binary
    outb(CONTROL02, 0b00110110);
    outb(COUNT0, (uint8_t)divisor);
    outb(COUNT0, (uint8_t)(divisor >> 8));
}

void timer_init(void)
{
    timer_set_freq(TIMER_FREQ);
}

void timer_handler(void)
{
    g_timer_elapsed += TIMER_SLICE;
}