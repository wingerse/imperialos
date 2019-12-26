#include "print.h"
#include "interrupts/interrupts.h"
#include "drivers/timer/timer.h"
#include "drivers/terminal/terminal.h"
#include "drivers/ata/ata.h"
#include "libc/string.h"
#include "def.h"

void kmain(void)
{
    timer_init();
    interrupt_init();
    ata_init();
    terminal_init();
    puts(
"                     WELCOME TO IMPERIAL OPERATING SYSTEM\n");
    puts("$ ");

    while (1) {}
}