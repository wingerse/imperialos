#pragma once

#include <stdint.h>

#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD   0xa0
#define PIC_SLAVE_DATA  0xa1

void interrupt_init(void);