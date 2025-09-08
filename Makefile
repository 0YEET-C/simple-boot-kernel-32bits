AS = nasm 
GCC = i686-elf-gcc
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy

ASFLAGS = -f elf32
CSFLAGS = -I kernel/include -I lib/include -ffreestanding -g -m32 -O0 -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

BOOT_DIR = boot
KERNEL_ASM_SRC = $(wildcard kernel/*.S)
KERNEL_SRC = $(wildcard kernel/*.c)
LIB_SRC = $(wildcard lib/*.c)
KERNEL_ASM_OBJ = $(patsubst kernel/%.S, build/%.o, $(KERNEL_ASM_SRC))
KERNEL_OBJ = $(patsubst kernel/%.c, build/%.o, $(KERNEL_SRC)) 
LIB_OBJ = $(patsubst lib/%.c, build/%.o, $(LIB_SRC))

STAGE1_BIN = build/stage1.bin
STAGE2_O = build/stage2.o
STAGE2_ELF = build/stage2.elf
STAGE2_BIN = build/stage2.bin
USER_BIN = build/user.bin
FINAL_IMAGE = test.bin

all: clean $(FINAL_IMAGE) 

build:
	mkdir -p build

$(STAGE1_BIN): $(BOOT_DIR)/stage1.nasm | build
	$(AS) -f bin -g $< -o $@

$(STAGE2_O): $(BOOT_DIR)/stage2.nasm | build
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: kernel/%.c | build
	$(GCC) $(CSFLAGS) -c $< -o $@

build/%.o: lib/%.c | build
	$(GCC) $(CSFLAGS) -c $< -o $@

build/%.o: kernel/%.S | build
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: user/%.c | build
	$(GCC) $(CSFLAGS_USER) -c $< -o $@

$(STAGE2_ELF): $(KERNEL_OBJ) $(STAGE2_O) $(LIB_OBJ) $(KERNEL_ASM_OBJ)
	$(LD) $(LDFLAGS) -o $@ $(STAGE2_O) $(KERNEL_OBJ) $(LIB_OBJ) $(KERNEL_ASM_OBJ)

$(STAGE2_BIN): $(STAGE2_ELF) 
	$(OBJCOPY) -O binary $< $@
	truncate -s 10240 $@

$(FINAL_IMAGE): $(STAGE1_BIN) $(STAGE2_BIN)
	cat $^ > $@

run: all
	qemu-system-i386 -drive format=raw,file=test.bin -serial stdio
	
clean:
	rm -rf build $(FINAL_IMAGE)

