ASMFLAGS += -felf32 -g
CFLAGS += -O0 -g3 -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -I../include
LIBS += -lgcc
LDFLAGS += -T kernel.ld -nostdlib -Wl,-melf_i386 -Wl

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.o, $(SOURCES_ASM))

.PHONY: all kernel clean

all: $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.elf: $(OBJECTS_ASM) $(OBJECTS_C)
	@$(LD) $(LDFLAGS),-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(LIBS)
	@echo "--> Created:  kernel.elf"
	@$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(BUILD_DIR)/kernel.sym
	@echo "--> Created:  kernel.sym"

$(BUILD_DIR)/kernel/c/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/kernel/asm/%.o: %.asm
	@mkdir -p $(@D)
	@$(ASM) $(ASMFLAGS) -o $@ $<
	@echo "--> Compiled: " $<

clean:
	@rm -f $(BUILD_DIR)/kernel.*
