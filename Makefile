
TARGET = edman_kernel
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump


SRCS += arch/arm/cortex-m4/startup.c
SRCS += kernel/main.c
SRCS += kernel/scheduler.c
SRCS += drivers/src/gpio.c
SRCS += drivers/src/uart.c
SRCS += drivers/src/spi.c
SRCS += drivers/src/adc.c

OBJS = $(SRCS:.c=.o)


INC = -Iinclude -Idrivers/include -Ikernel

# (WALL OF TERROR) 

CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O2 -g -Wall -Wextra -Werror -Wshadow -Wundef -Wconversion -std=c99 $(INC)

# 
LDFLAGS = -Tarch/arm/cortex-m4/stm32f446.ld --specs=nano.specs -nostartfiles -Wl,--gc-sections

# 
all: $(TARGET).bin $(TARGET).list
	@echo "========================================="
	@echo "   BUILD SUCCESSFUL (Wall of Terror Passed) "
	@echo "========================================="


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@


$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@


$(TARGET).list: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@


clean:
	@rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).list
	@echo "Rensat och fint!"


flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

.PHONY: all clean flash
