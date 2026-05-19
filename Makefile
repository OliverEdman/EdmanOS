# --- Projektinställningar ---
TARGET = edman_kernel
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# --- Sökvägar (Paths) ---
# Här talar vi om för Make exakt var alla källfiler finns
SRCS += arch/arm/cortex-m4/startup.c
SRCS += kernel/main.c
SRCS += kernel/scheduler.c
SRCS += drivers/src/gpio.c
SRCS += drivers/src/uart.c
SRCS += drivers/src/spi.c
SRCS += drivers/src/adc.c

# Generera automatiskt .o-filer i samma mappar som .c-filerna
OBJS = $(SRCS:.c=.o)

# --- Include-flaggor (-I) ---
# Talar om för kompilatorn var den ska leta efter .h-filer när du skriver #include
INC = -Iinclude -Idrivers/include -Ikernel

# --- Kompilator-flaggor ---
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
         -O0 -g -Wall -std=c99 $(INC)

# --- Linker-flaggor ---
# Notera att vi har uppdaterat sökvägen till din .ld-fil här!
LDFLAGS = -Tarch/arm/cortex-m4/stm32f446.ld --specs=nano.specs -nostartfiles -Wl,--gc-sections

# --- Regler för att bygga ---
all: $(TARGET).bin

# Kompilera varje enskild .c-fil till en .o-fil
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Länka samman alla .o-filer till den slutgiltiga .elf-filen
$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Skapa den rena binärfilen som ST-LINK förstår
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# --- Rensa gamla byggfiler ---
clean:
	@rm -f $(OBJS) $(TARGET).elf $(TARGET).bin
	@echo "Rensat och fint!"

# --- Flasha till din Nucleo via USB ---
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

.PHONY: all clean flash