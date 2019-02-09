C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h common/*.h cpu/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/Makefile,%,$(mkfile_path))

# toolchains
CC = ${current_dir}/toolchain/bin/i386-elf-gcc
GDB = ${current_dir}/toolchain/bin/i386-elf-gdb
LD = ${current_dir}/toolchain/bin/i386-elf-ld

# -g: Use debugging symbols in gcc
CFLAGS = -g

# concatenate two binary file
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
