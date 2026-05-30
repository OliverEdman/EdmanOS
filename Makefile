TARGET = edman_kernel
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

#  Kataloger 
BUILD_DIR = build
INCLUDE_DIR = include

#  Källkodsfiler
SRCS += arch/arm/cortex-m4/startup.c
SRCS += kernel/main.c
SRCS += kernel/scheduler.c
SRCS += drivers/stm32f446_nucleo/gpio.c
SRCS += drivers/stm32f446_nucleo/uart.c
SRCS += drivers/stm32f446_nucleo/spi.c
SRCS += drivers/stm32f446_nucleo/adc.c

# Objektfiler (Placeras nu inuti build-mappen)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# Inkluderingsflaggor
INC = -I$(INCLUDE_DIR)

#  Kompileringsflaggor (The Wall of Terror) 
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O2 -g -Wall -Wextra -Werror -Wshadow -Wundef -Wconversion -std=c99 $(INC)

#  Länkarflaggor 
LDFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -Tarch/arm/cortex-m4/stm32f446.ld --specs=nano.specs -nostartfiles -Wl,--gc-sections

# Huvudmål
all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).list
	@echo ""
	@echo "========================================="
	@echo " EdmanOS built successfully!"
	@echo "========================================="
	@echo ""

# Regel för att kompilera .c-filer till .o-filer inuti build
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#  Länka ihop till .elf 
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

#  Generera rå binärfil (.bin) 
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

#  Generera assembler-listning (.list)
$(BUILD_DIR)/$(TARGET).list: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

#  Rensa allt byggskräp (Raderar hela build-mappen) 
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete! /build directory removed."

# Flasha till kortet 
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x08000000

	@echo ""
	@echo "=================================================="
	@echo "EdmanOS flashed successfully to target!"
	@echo "=================================================="
	@echo " Memory Usage:"
	@# Hämta värden från size-verktyget via awk
	@eval $$(arm-none-eabi-size $(BUILD_DIR)/$(TARGET).elf | awk 'NR==2 {print "TEXT="$$1 "; DATA="$$2 "; BSS="$$3}') ; \
	FLASH_USED=$$(($$TEXT + $$DATA)) ; \
	RAM_USED=$$(($$DATA + $$BSS)) ; \
	FLASH_TOTAL=524288 ; \
	RAM_TOTAL=131072 ; \
	FLASH_PCT=$$(($$FLASH_USED * 100 / $$FLASH_TOTAL)) ; \
	RAM_PCT=$$(($$RAM_USED * 100 / $$RAM_TOTAL)) ; \
	echo "  FLASH: $$FLASH_USED / $$FLASH_TOTAL bytes [$$FLASH_PCT% Used]" ; \
	echo "  SRAM:  $$RAM_USED / $$RAM_TOTAL bytes [$$RAM_PCT% Used]" ; \



.PHONY: all clean flash
