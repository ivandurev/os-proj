SRC_ROOT = src
BUILD_ROOT = build
FOLDERS = drivers irq kernel mem schedule user utils
KERNEL_NAME = kernel8


C_FILES = $(foreach f,$(FOLDERS),$(wildcard $(SRC_ROOT)/$(f)/*.c))
ASM_FILES = $(foreach f,$(FOLDERS),$(wildcard $(SRC_ROOT)/$(f)/*.S))
O_FILES = $(C_FILES:$(SRC_ROOT)/%.c=$(BUILD_ROOT)/%.o) $(ASM_FILES:$(SRC_ROOT)/%.S=$(BUILD_ROOT)/%_asm.o)

GCC_FLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles -Iinclude -DDEBUG -mgeneral-regs-only -include stdint.h -include stdbool.h	-include stddef.h # can possibly allow FP and SIMD in the future
ASM_FLAGS = -Iinclude -DDEBUG
LD_FLAGS = -nostdlib

all: clean folders boot/$(KERNEL_NAME).img

folders:
	$(foreach f,$(FOLDERS),$(shell mkdir $(BUILD_ROOT)\$(f) >nul 2>nul))

$(BUILD_ROOT)/%_asm.o: $(SRC_ROOT)/%.S
	aarch64-none-elf-gcc $(ASM_FLAGS) -MMD -c $< -o $@


$(BUILD_ROOT)/%.o: $(SRC_ROOT)/%.c
	aarch64-none-elf-gcc $(GCC_FLAGS) -MMD -c $< -o $@

-include $(O_FILES:%.o=%.d)

boot/$(KERNEL_NAME).img: $(SRC_ROOT)/linker.ld $(O_FILES)
	aarch64-none-elf-ld $(O_FILES) -T $(SRC_ROOT)/linker.ld -o $(BUILD_ROOT)/$(KERNEL_NAME).elf
	aarch64-none-elf-objcopy -O binary $(BUILD_ROOT)/$(KERNEL_NAME).elf boot/$(KERNEL_NAME).img

clean:
	rmdir /Q /s $(BUILD_ROOT) >nul 2>nul
	del "boot\$(KERNEL_NAME).img" >nul 2>nul

run:
	qemu-system-aarch64 -M raspi3b -kernel boot/$(KERNEL_NAME).img -serial null -serial stdio
debug:
	qemu-system-aarch64 -M raspi3b -d in_asm -kernel boot/$(KERNEL_NAME).img -serial null -serial stdio -monitor stdio -S -nographic