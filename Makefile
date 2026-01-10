#  QUEOS version
VERSION = 0.0.1

PREFIX = i686-elf-

export ASM = nasm
export CC = $(PREFIX)gcc
export LD = $(PREFIX)gcc
export OBJDUMP = $(PREFIX)objdump
export OBJCOPY = $(PREFIX)objcopy

export CFLAGS = -std=gnu11 -Wall -Wextra -DVERSION=\"$(VERSION)\"
export LDFLAGS =
export ASMFLAGS =
export LIBS =

export BUILD_DIR = $(abspath build)

QEMU = qemu-system-i386

.PHONY: grub bochs qemu qemu-nox qemu-gdb qemu-nox-gdb clean always 

all: $(BUILD_DIR)/queos.img

$(BUILD_DIR)/queos.img: $(BUILD_DIR)/bootsec.bin $(BUILD_DIR)/kernel.elf
	@dd if=/dev/zero of=$(BUILD_DIR)/qos.img count=10200 > /dev/null 2>&1
	@dd if=$(BUILD_DIR)/bootsec.bin of=$(BUILD_DIR)/queos.img conv=notrunc > /dev/null 2>&1
	@dd if=$(BUILD_DIR)/kernel.elf of=$(BUILD_DIR)/queos.img seek=1 conv=notrunc > /dev/null 2>&1
	@echo "\033[36;1mCreated bootable disk image "\""queos.img"\""\033[0m"

grub: $(BUILD_DIR)/queos.iso

$(BUILD_DIR)/queos.iso: $(BUILD_DIR)/kernel.elf
	@cp $< iso/boot/
	@mkisofs -R -b boot/grub/grub -no-emul-boot -boot-load-size 4 \
           -A os -input-charset utf8 -quiet -boot-info-table -o $@ iso

$(BUILD_DIR)/bootsec.bin: always
	@$(MAKE) -C boot/ BUILD_DIR=$(abspath $(BUILD_DIR))

$(BUILD_DIR)/kernel.elf: always
	@$(MAKE) -C kernel/ BUILD_DIR=$(abspath $(BUILD_DIR))

bochs: all
	echo c | bochs -q

qemu: all
	@echo Ctrl+a h for help
	@$(QEMU) -serial mon:stdio -hda $(BUILD_DIR)/queos.img -m 32

qemu-nox: all
	@echo Ctrl+a h for help
	@$(QEMU) -nographic -hda $(BUILD_DIR)/queos.img -m 32

qemu-gdb: all
	@echo "Now run 'gdb' from another terminal." 1>&2
	@echo Ctrl+a h for help
	@$(QEMU) -serial mon:stdio -hda $(BUILD_DIR)/queos.img -m 32 -S -gdb tcp::25501

qemu-nox-gdb: all
	@echo "Now run 'gdb' from another terminal." 1>&2
	@echo Ctrl+a h for help
	@$(QEMU) -nographic -hda $(BUILD_DIR)/queos.img -m 32 -S -gdb tcp::25501

always:
	@mkdir -p $(BUILD_DIR)

clean:
	@$(MAKE) -C boot/ BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	@$(MAKE) -C kernel/ BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	@rm -rf $(BUILD_DIR)/*
	@rm -rf iso/boot/*.elf
