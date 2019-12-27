#include "terminal.h"
#include <stddef.h>
#include "../../x86.h"
#include "../../print.h"
#include "../timer/timer.h"
#include <stdbool.h>
#include "../../libc/string.h"
#include "../ata/ata.h"

#define VGA_ADDRESS       0x3d4
#define VGA_DATA          0x3d5
#define VGA_CURSOR_LOC_HI 0x0e
#define VGA_CURSOR_LOC_LO 0x0f
#define VGA_CURSOR_START  0x0a
#define VGA_CURSOR_END    0x0b

#define VIDEO_START 0xb8000
#define VIDEO ((volatile char *)VIDEO_START)
#define WIDTH 80 
#define HEIGHT 25
#define SIZE (WIDTH*HEIGHT)

#define BUFFER_SIZE SIZE

typedef struct {
    int x;
    int y;
} pos;

static int pos_to_index(pos p)
{
    return p.y * WIDTH + p.x;
}

static pos index_to_pos(int i)
{
    return (pos){i % WIDTH, i / WIDTH};
}

static int cursor_index;
static int in_start_index;
static int out_start_index;

static int input_length(void)
{
    return out_start_index - in_start_index;
}

static int input_index(void)
{
    return cursor_index - in_start_index;
}

static char input_buffer[BUFFER_SIZE];

static void enable_cursor(void)
{
    outb(VGA_ADDRESS, VGA_CURSOR_START);
    outb(VGA_DATA, 14);
    outb(VGA_ADDRESS, VGA_CURSOR_END);
    outb(VGA_DATA, 15);
}

static void update_cursor(void)
{
    uint16_t i = (uint16_t)cursor_index;

    outb(VGA_ADDRESS, VGA_CURSOR_LOC_LO);
    outb(VGA_DATA, (uint8_t)i);
    outb(VGA_ADDRESS, VGA_CURSOR_LOC_HI);
    outb(VGA_DATA, (uint8_t)(i >> 8));
}

void terminal_init(void)
{
    terminal_clear();
    enable_cursor();
    update_cursor();
}

static void set_char(int index, char c, uint8_t color)
{
    VIDEO[index*2] = c;
    VIDEO[index*2 + 1] = (char)color;
}

void terminal_clear(void)
{
    for (int i = 0; i < SIZE; i++) {
        set_char(i, ' ', COLOR_WB);
    }
}

static void scroll_up(void)
{
    volatile uint32_t *dst = (volatile uint32_t *)VIDEO_START;
    volatile uint32_t *src = (volatile uint32_t *)VIDEO_START + WIDTH*2/4;
    // memmove the rows up.
    for (size_t i = 0; i < (HEIGHT-1)*WIDTH*2/4; i++) {
        *dst++ = *src++;
    }
    // for the last row, make them empty cells
    int index = (int)((uintptr_t)dst - VIDEO_START)/2;
    for (int i = 0; i < WIDTH; i++) {
        set_char(index + i, ' ', COLOR_WB);
    }
    pos p = index_to_pos(cursor_index);
    p.y--;
    cursor_index = pos_to_index(p);

    // yes it can get negative
    p = index_to_pos(in_start_index);
    p.y--;
    in_start_index = pos_to_index(p);

    p = index_to_pos(out_start_index);
    p.y--;
    out_start_index = pos_to_index(p);
}

static void got_input(void)
{
    in_start_index = cursor_index;
    out_start_index = in_start_index;
    size_t len = strlen(input_buffer);
    bool has_newline = len && input_buffer[len-1] == '\n';
    if (has_newline) {
        input_buffer[len-1] = '\0';
    }
    if (strncmp(input_buffer, "echo ", 5) == 0) {
        kprintf(input_buffer+5);
        putc('\n');
    } else if (strcmp(input_buffer, "shutdown") == 0) {
        kprintf("Oh no you don't ;)\n");
    } else if (strcmp(input_buffer, "ls") == 0) {
        kprintf("emperor\naiman\n");
    } else if (strcmp(input_buffer, "pwd") == 0) {
        kprintf("universe\n");
    } else if (strcmp(input_buffer, "read") == 0) {
        char buf[11];
        int err = ata_pio_read(0, buf, 11);
        if (err) {
            kprintf("Error reading hard disk\n");
        } else {
            for (int i = 0; i < 11; i++) {
                putc(buf[i]);
            }
        }
    } else {
        kprintf("%s: command found but I will not execute it (:\n", input_buffer);
    }
    kprintf("$ ");
}

void terminal_move_left(void)
{
    if (cursor_index == in_start_index || out_start_index < in_start_index) {
        return;
    }
    if (cursor_index != 0) {
        cursor_index--;
    }
    update_cursor();
}

void terminal_move_right(void)
{
    if (cursor_index == out_start_index || out_start_index < in_start_index) {
        return;
    }
    if (cursor_index != SIZE-1) {
        cursor_index++;
    }
    update_cursor();
}

void terminal_in_putc(char c)
{
    if (out_start_index < in_start_index) {
        out_start_index = in_start_index;
    }

    if (c == '\b') {
        if (cursor_index == in_start_index || cursor_index == 0) {
            return;
        }

        memmove(input_buffer+input_index()-1, input_buffer+input_index(), (size_t)(input_length()-input_index()));
        cursor_index--;
        out_start_index--;
        update_cursor();
        for (int i = input_index(); i < input_length(); i++) {
            set_char(in_start_index + i, input_buffer[i], COLOR_WB);
        }
        set_char(in_start_index + input_length(), ' ', COLOR_WB); 

        return;
    }

    bool _got_input = false;

    if (c == '\n') {
        input_buffer[input_length()] = '\n';
        input_buffer[input_length()+1] = '\0';
        pos p = index_to_pos(cursor_index);
        p.x = 0;
        p.y++;
        cursor_index = pos_to_index(p);
        if (cursor_index >= SIZE) {
            scroll_up();
        }
        update_cursor();
        got_input();
    } else {
        memmove(input_buffer+input_index()+1, input_buffer+input_index(), (size_t)(input_length()-input_index()));
        input_buffer[input_index()] = c;
        out_start_index++;

        for (int i = input_index(); i < input_length(); i++) {
            set_char(in_start_index + i, input_buffer[i], COLOR_WB);
        }
        cursor_index++;

        if (in_start_index + input_length() >= SIZE) {
            scroll_up();
        }

        update_cursor();
        if (input_length() == BUFFER_SIZE-1) {
            input_buffer[input_length()] = '\0';
            got_input();
        }
    }
}

void terminal_out_putc(char c)
{
    if (c == '\b') {
        if (cursor_index == out_start_index || cursor_index == 0) {
            return;
        }
        cursor_index--;
        update_cursor();
        in_start_index--;
        set_char(cursor_index, ' ', COLOR_WB); 
        return;
    }

    if (c == '\n') {
        pos p = index_to_pos(cursor_index);
        p.x = 0;
        p.y++;
        cursor_index = pos_to_index(p);
    } else {
        set_char(cursor_index, c, COLOR_WB);
        cursor_index++;
    }

    if (cursor_index >= SIZE) {
        scroll_up();
    }

    update_cursor();

    in_start_index = cursor_index;
}