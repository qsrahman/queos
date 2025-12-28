CFLAGS += -O -ffreestanding -nostdlib -I../include
LDFLAGS += -nostdlib
ASMFLAGS += -felf32

.PHONY: all clean

all: $(BUILD_DIR)/bootsec.elf

$(BUILD_DIR)/bootsec.elf: $(BUILD_DIR)/boot/asm/bootsec.o $(BUILD_DIR)/boot/c/bootmain.o
	@$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o $@ $^
	@$(OBJCOPY) -S -O binary -j .text $@ $(BUILD_DIR)/bootsec.bin > /dev/null 2>&1
	@./sign.pl $(BUILD_DIR)/bootsec.bin

$(BUILD_DIR)/boot/c/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/boot/asm/%.o: %.asm
	@mkdir -p $(@D)
	@$(ASM) $(ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

clean:
	@rm -f $(BUILD_DIR)/boot.*
