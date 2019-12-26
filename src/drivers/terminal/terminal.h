#pragma once
#include <stdint.h>

enum {
    COLOR_BLACK,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_RED,
    COLOR_MAGENTA,
    COLOR_BROWN,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_GRAY,
    COLOR_LIGHT_BLUE,
    COLOR_LIGHT_GREEN,
    COLOR_LIGHT_CYAN,
    COLOR_LIGHT_RED,
    COLOR_LIGHT_MAGENTA,
    COLOR_YELLOW,
    COLOR_WHITE
};

#define COLOR(fg, bg) (uint8_t)((bg) << 8 | (fg))
#define COLOR_WB COLOR(COLOR_LIGHT_GRAY, COLOR_BLACK)

void terminal_init(void);
void terminal_clear(void);
void terminal_move_left(void);
void terminal_move_right(void);
// for user input
void terminal_in_putc(char c);
// for all other output
void terminal_out_putc(char c);