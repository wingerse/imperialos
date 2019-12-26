extern start

section .text
bits 16 ; CPU starts in real mode
    ; cs:ip = 0x7c00
    ; the stack is usable     
    cli ; disable interrupts
    
    ; enable A20 using fast method
    ; out(0x92, in(0x92) | 0x2)
    in al, 0x92 
    or al, 0x2
    out 0x92, al

    ; load gdt
    lgdt [gdt_desc] ; this instruction does not use segment registers, so no need to clear ds

    ; enable protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; "clear" cs, as in make it identity translation
    jmp 8:start32  ; 8 is the offset of code SEG_DESC

bits 32
start32:
    ; make ds, es, ss also identity translations
    mov ax, 16 ; 16 is the offset of data SEG_DESC
    mov ds, ax
    mov es, ax
    mov ss, ax
    ; don't care about fs and gs for now
    xor ax, ax
    mov fs, ax
    mov gs, ax

    ; 31KB for the stack, right below the boot loader
    mov esp, 0x7c00    
    ; now I can call into C
    call start

    ; C should NEVER return.
    .l:
        hlt
        jmp .l

section .data
gdt_desc:
    dw (gdt_end - gdt - 1)
    dd gdt

; present bit and descriptor type bit already set
; granularity = 4KiB, and 32 bit mode set.
%define SEG_DESC(base, limit, access) \
       dq ((limit & 0xffff) | \
          (base & 0xffff) << 16 | \
          ((base >> 16) & 0xff) << 32 | \
          (0b10010000 | access) << 40 | \
          ((limit >> 16) & 0xf) << 48 | \
          (0b1100) << 52 | \
          ((base >> 24) & 0xff) << 56)

%define SEG_AC_PRIVL(privl) (privl << 5)
%define SEG_AC_EX (1 << 3)
%define SEG_AC_DC (1 << 2)
%define SEG_AC_RW (1 << 1)

; don't care about segmentation (use paging instead). Make everything identity translation
gdt:
    dq 0 ; null descriptor
    SEG_DESC(0, 0xfffff, SEG_AC_RW | SEG_AC_EX) ; code
    SEG_DESC(0, 0xfffff, SEG_AC_RW) ; data
gdt_end: