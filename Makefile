BUILD_DIR = build
SRC_DIR = src

KERNEL_NAME = kernel8

C_FILES = $(wildcard $(SRC_DIR)/c/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/asm/*.S)
O_FILES = $(ASM_FILES:$(SRC_DIR)/asm/%.S=$(BUILD_DIR)/%_asm.o) $(C_FILES:$(SRC_DIR)/c/%.c=$(BUILD_DIR)/%.o)

GCC_FLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles -Iinclude -mgeneral-regs-only # can possibly allow FP and SIMD in the future
ASM_FLAGS = -Iinclude
LD_FLAGS = -nostdlib

all: clean boot/$(KERNEL_NAME).img

$(BUILD_DIR)/%_asm.o: $(SRC_DIR)/asm/%.S
	aarch64-none-elf-gcc $(ASM_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/c/%.c
	aarch64-none-elf-gcc $(GCC_FLAGS) -MMD -c $< -o $@

-include $(O_FILES:%.o=%.d)

boot/$(KERNEL_NAME).img: $(SRC_DIR)/linker.ld $(O_FILES)
	aarch64-none-elf-ld $(O_FILES) -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/$(KERNEL_NAME).elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/$(KERNEL_NAME).elf boot/$(KERNEL_NAME).img

clean:
	del /S /Q $(BUILD_DIR) "$(KERNEL_NAME).img"

run:
	qemu-system-aarch64 -M raspi3b -kernel boot/$(KERNEL_NAME).img -serial null -serial stdio