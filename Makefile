CC = ${CROSS_COMPILER_PATH}

STORM_CPPFLAGS += -Isrc \
	          -Isrc/kernel \
	          -Isrc/kernel/arch/i386 \
	          -Isrc/kernel/arch/i386/cpu \
	          -Isrc/lib

STORM_CFLAGS = -Werror -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wshadow
STORM_CFLAGS += -std=gnu99
STORM_CFLAGS += -O0
STORM_CFLAGS += -g
STORM_CFLAGS += -ffreestanding
STORM_CFLAGS += -fno-strict-aliasing

STORM_LDFLAGS += -nostdlib
STORM_LDFLAGS += -T src/kernel/arch/i386/link.ld

LIBS = -lgcc

BINARY = storm
all: $(BINARY)

SOURCES = \
	src/kernel/arch/i386/boot_asm.S \
	src/kernel/arch/i386/io_asm.S \
	src/kernel/arch/i386/i8259.c \
	src/kernel/arch/i386/uart.c \
	src/kernel/arch/i386/cpu/cpu.c \
	src/kernel/arch/i386/cpu/cpu_asm.S \
	src/kernel/arch/i386/cpu/gdt.c \
	src/kernel/arch/i386/cpu/gdt_asm.S \
	src/kernel/arch/i386/cpu/idt.c \
	src/kernel/arch/i386/cpu/idt_asm.S \
	src/kernel/arch/i386/cpu/interrupts.c \
	src/kernel/arch/i386/cpu/interrupts_asm.S \
	src/kernel/heap.c \
	src/kernel/main.c \
	src/kernel/panic.c \
	src/kernel/putchar.c \
	src/lib/printf.c

OBJECTS = $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SOURCES)))

$(BINARY): $(OBJECTS)
	@$(CC) $(STORM_LDFLAGS) $^ $(LIBS) -o $@
	@echo "Link $(BINARY)"

%.o: %.c
	@$(CC) $(STORM_CPPFLAGS) $(STORM_CFLAGS) -c -o $@ $<
	@echo "CC $<"

%.o: %.S
	@$(CC) $(STORM_CPPFLAGS) $(STORM_CFLAGS) -c -o $@ $<
	@echo "CC $<"

clean:
	rm -f $(BINARY) $(OBJECTS)

run_qemu:
	@qemu-system-i386 \
	-gdb tcp::1234 \
        -nographic \
        -kernel storm

run_qemu_halt:
	@qemu-system-i386 \
	-gdb tcp::1234 \
        -nographic \
        -S \
        -kernel storm

.PHONY: clean run_qemu
