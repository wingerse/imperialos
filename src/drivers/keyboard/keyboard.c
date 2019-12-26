#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>
#include "../../x86.h"
#include "../../print.h"
#include "../timer/timer.h"
#include "../terminal/terminal.h"

#define DATA    0x60
#define COMMAND 0x64
#define STATUS  0x64

#define STATUS_PARITY_ERROR            0x80
#define STATUS_GENERAL_TIMEOUT         0x40
#define STATUS_AUX_DEV_OUTPUT_BUF_FULL 0x20
#define STATUS_INHIBIT_SWITCH          0x10
#define STATUS_CMD_DATA                0x08
#define STATUS_SYS_FLAG                0x04
#define STATUS_INPUT_BUF_FULL          0x02
#define STATUS_OUTPUT_BUF_FULL         0x01

#define KEY_PRESS(scancode)   (!(scancode & 0x80))
#define KEY_RELEASE(scancode) (!KEY_PRESS(scancode))

char codeset[128] =
{
    0,'\033','1','2','3','4','5','6','7','8','9','0','-','=',0x08,
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\\',0,0,0,
};	

char codeset_shift[128] =
{
    0,'\033','!','@','#','$','%','^','&','*','(',')','_','+',0x08,
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',
    0,0,0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\\',0,0,0,
};

typedef struct {
    bool shift: 1;
    bool ctrl:  1;
    bool alt:   1;
} keyboard_state;

static keyboard_state state;

void keyboard_handler(void)
{
    uint8_t scancode = inb(DATA);
    bool pressed = KEY_PRESS(scancode);
    scancode &= 0x7f;

    switch (scancode) {
    case 0x2a:
    case 0x36:
        state.shift = pressed;
        return;
    case 0x1d:
        state.ctrl = pressed;
        return;
    case 0x38:
        state.alt = pressed;
        return;
    }

    if (!pressed) {
        return;
    }

    switch (scancode) {
    case 0x4b:
        terminal_move_left();
        break;
    case 0x4d:
        terminal_move_right();
        break;
    default:;
        char c = state.shift ? codeset_shift[scancode] : codeset[scancode];
        terminal_in_putc(c);
    }
}