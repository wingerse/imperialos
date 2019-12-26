#pragma once

typedef struct interrupt_frame interrupt_frame;

__attribute__((interrupt))
void hardware_int_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq0_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq1_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq2_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq3_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq4_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq5_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq6_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq7_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq8_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq9_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq10_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq11_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq12_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq13_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq14_handler(interrupt_frame *frame);
__attribute__((interrupt))
void irq15_handler(interrupt_frame *frame);
__attribute__((interrupt))
void syscall_handler(interrupt_frame *frame);