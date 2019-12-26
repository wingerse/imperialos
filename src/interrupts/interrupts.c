#include "interrupts.h"
#include "handlers.h"
#include <stdint.h>
#include "../x86.h"

typedef struct __attribute__((packed)) {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  attr;
    uint16_t offset_hi;
} idt_ent;

typedef struct __attribute__((packed)) {
    uint16_t length;
    uint32_t base;
} idt_desc;

#define IDT_ENT(addr) (idt_ent){(uint16_t)((uintptr_t)addr&0xffff), 8, 0, 0x8e, (uint16_t)((uintptr_t)addr>>16)}

static idt_ent idt[256];

static idt_desc desc = {
    sizeof(idt)-1,
    (uint32_t)(uintptr_t)idt,
};

static void setup_idt(void) 
{
    for (int i = 0; i < 0x1f; i++) {
        idt[i] = IDT_ENT(hardware_int_handler);
    }
    idt[0x20] = IDT_ENT(irq0_handler);
    idt[0x21] = IDT_ENT(irq1_handler);
    idt[0x22] = IDT_ENT(irq2_handler);
    idt[0x23] = IDT_ENT(irq3_handler);
    idt[0x24] = IDT_ENT(irq4_handler);
    idt[0x25] = IDT_ENT(irq5_handler);
    idt[0x26] = IDT_ENT(irq6_handler);
    idt[0x27] = IDT_ENT(irq7_handler);

    idt[0x28] = IDT_ENT(irq8_handler);
    idt[0x29] = IDT_ENT(irq9_handler);
    idt[0x2a] = IDT_ENT(irq10_handler);
    idt[0x2b] = IDT_ENT(irq11_handler);
    idt[0x2c] = IDT_ENT(irq12_handler);
    idt[0x2d] = IDT_ENT(irq13_handler);
    idt[0x2e] = IDT_ENT(irq14_handler);
    idt[0x2f] = IDT_ENT(irq15_handler);

    idt[0x80] = IDT_ENT(syscall_handler);
}

void interrupt_init(void)
{
    // initialize the PIC
    setup_idt();

    outb(PIC_MASTER_CMD, 0x19);
    // int 0x00 - 0x1f is reserved for cpu
    // so we move master to 0x20 and slave to 0x28
    // not really required for slave but who cares, looks better
    outb(PIC_MASTER_DATA, 0x20);
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_MASTER_DATA, 0x01);

    outb(PIC_SLAVE_CMD, 0x19);
    outb(PIC_SLAVE_DATA, 0x28);
    outb(PIC_SLAVE_DATA, 0x02);
    outb(PIC_SLAVE_DATA, 0x01);

    // load idt and enable interrupts
    asm volatile("lidt %0\n\t"
                 "sti":
                 :
                 "m"(desc):
                 "cc");
}