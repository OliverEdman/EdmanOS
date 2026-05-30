### Edman Kernel

A custom, bare-metal real time operating system (RTOS) designed, architected, and implemented entirely from scratch for the ARM Cortex-M4 architecture. This is a **solo project** where I have one goal in mind: to learn and discover exactly what happens under the hood of a modern microprocessor. 

### 🛠️ Hardware Stack & Environment
- **Development Board:** Nucleo-F446RE
- **Processor:** STM32F446RE (ARM Cortex-M4 with a Hardware Floating Point Unit / FPU)
- **Editor:** Neovim (Nvim)
- **Toolchain:** `arm-none-eabi-gcc` & `st-flash`


### 🚀 Project & Learning Goals
- **True Bare-Metal Development:** Writing custom drivers, startup code, and linker scripts completely from scratch without using auto generated code tools.
- **Time Critical Systems:** Understanding how a scheduler manages thread switching, determinism, and clock cycles.
- **Resource Management:** Implementing a Task Manager that strictly controls which threads or tasks have access to specific hardware resources.
- **Thread Safety & Concurrency:** Implementing atomic variables, synchronization primitives, and barriers to prevent race conditions in a multithreaded environment.

### 🛠️ Hardware Specification
The project is strictly designed for and tested on the following microcontroller:
- **Development Board:** Nucleo-F446RE
- **Processor:** STM32F446RE (ARM Cortex-M4 with a Hardware Floating Point Unit / FPU)

### 📂 System Architecture & Components

### Boot & Linking
- **`arch/arm/cortex-m4/startup.c`**: Initializes the stack pointer, clears the `.bss` section, copies the `.data` section from Flash to SRAM, and sets up the Vector Interrupt Table.
- **`arch/arm/cortex-m4/stm32f446.ld`**: Custom linker script mapping the processor's Flash (512 KB) and SRAM (128 KB) locations precisely in memory.

### Kernel & Scheduler
- **Scheduler (`kernel/scheduler.c`)**: Manages context switching by saving and restoring CPU registers onto individual task stacks during a timer interrupt.
- **Task Manager**: An access control mechanism regulating execution privileges and task states.
- **Thread Safety**: Built-in atomic operations to ensure safe data sharing between concurrent threads and Interrupt Service Routines (ISRs).

###  Register Based Drivers (`drivers/`)
All peripherals are configured via direct Memory-Mapped I/O (MMIO) register manipulation:
- **GPIO**: Pin configuration (Input, Output, Alternate Function, Analog) utilizing custom bitmask macros to avoid magic numbers.
- **UART**: Polling and interrupt-driven serial communication for kernel logging and debugging (115200 baud).
- **SPI / I2C**: Hardware bus protocols for interfacing with external sensors (e.g., distance sensors for autonomous vehicles).
- **ADC**: Analog-to-Digital conversion for reading analog sensor signals in real time.

### ⚡ Compilation & Flashing

The project is compiled using an warning configuration (**Wall of Terror**) to guarantee code quality and catch potential bugs at compile time.

### Toolchain
- **Compiler:** `arm-none-eabi-gcc` (C99 Standard)
- **Flashing Utility:** `st-flash` (ST-LINK v2-1)

### Build Commands
To clean previous build files, compile the entire kernel, and flash the binary to the Nucleo board, execute:

```bash
make clean && make && make flash
