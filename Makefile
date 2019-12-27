CC = gcc
CFLAGS += -m32 -O2 -masm=intel -std=gnu11 \
-Werror -Wall -Wextra -Wconversion \
-nostdlib -ffreestanding -fno-pic\
-Wno-unused-function -Wno-unused-variable

src_asm = $(shell find src -name '*.nasm' -print | paste -s -)
src_c   = $(shell find src -name '*.c' -print | paste -s -)

$(shell mkdir -p bin)

obj_asm = $(patsubst src/%.nasm,obj/%.asm.o,$(src_asm))
obj_c   = $(patsubst src/%.c,obj/%.o,$(src_c))

bin/kernel: $(obj_asm) $(obj_c) kernel.ld bin/fs
	$(CC) $(CFLAGS) -o bin/kernel $(obj_asm) $(obj_c) -T kernel.ld -Wl,--oformat,binary,-m,elf_i386 -lgcc

bin/fs:
	echo "Hello world" > bin/fs

obj/interrupts/handlers.o: src/interrupts/handlers.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -mgeneral-regs-only -MP -MMD

obj/%.asm.o: src/%.nasm
	mkdir -p $(@D)
	nasm -f elf32 -o $@ $<

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) -MP -MMD

run: bin/kernel
	qemu-system-i386 -drive file=bin/kernel,format=raw,media=disk,index=0 -drive file=bin/fs,format=raw,media=disk,index=1

.PHONY: run
.PHONY: clean

clean:
	rm -rf obj/

-include $(obj_c:.o=.d)
