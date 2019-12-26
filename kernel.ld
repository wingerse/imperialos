SECTIONS {
    /* boot loader is loaded into 0x7c00 */
    . = 0x7c00;
    .btext : AT(0) { 
        obj/boot/boot.asm.o(.text)
        obj/boot/boot.o(.text)
    } 
    .bdata : { 
        obj/boot/boot.asm.o(.data)
        obj/boot/boot.o(.data)
    } 
    .brodata : {
        obj/boot/boot.asm.o(.rodata)
        obj/boot/boot.o(.rodata)
    } 
    .bsig : AT(510) {
        BYTE(0x55)
        BYTE(0xaa)
    } 
    /* kernel is loaded into 0x100000. */
    L_KERNEL_START = 0x100000;
    . = L_KERNEL_START;
    .ktext : AT(512) {
        *(.text)
    }
    .kdata : {
        *(.data)
    }
    .krodata : { 
        *(.rodata .rodata.*)
    }
    L_BSS_START = .;
    .kbss : { 
        *(.bss)
        *(COMMON)
    }
    L_BSS_SIZE = . - L_BSS_START;
    L_KERNEL_SIZE = . - L_KERNEL_START;
    /DISCARD/ : {
        *(.comment)
        *(.note.GNU-stack)
        *(.eh_frame)
    }
}