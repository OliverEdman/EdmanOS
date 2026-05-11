/**
 * Edman Kernel - Startup Code (Cortex-M4 / STM32F446)
 * --------------------------------------------------
 * This file is the kernel's "alarm clock". When the processor gets power, it knows nothing.
 * It is hardcoded to look at address 0x08000000 in the Flash memory for instructions.
 * * The processor expects a so-called "Vector Table" where:
 * 1. First 4 bytes (Index 0) = Where the stack starts (Initial Stack Pointer).
 * 2. Next 4 bytes (Index 1)  = Where the code starts (Reset Handler).
 */

#include <stdint.h>

// Top of Block 1 (SRAM). The stack grows downwards from here.
// A 32-bit pointer pointing to the address where the top of the stack is.
#define STACK_TOP (uint32_t *)0x20020000 

// 'extern' means that main() exists in another file (e.g., kernel/main.c)
extern int main(void);
void Reset_Handler(void);

// Struct that matches exactly what the hardware expects.
typedef struct {
    uint32_t *initial_sp_value;   // Address for Stack Pointer (SP)
    void (*reset_handler)(void);  // Address to the first instruction (PC)
} VectorTable;

/**
 * __attribute__((section(".isr_vector"))) 
 * __attribute is not part of standard C but is an addition in GCC. 
 * The compiler has a standard way to store and decide where things should be placed,
 * but here we tell the compiler: "I want to decide for myself where the vector table should be,"
 * at address 0x08000000, so that the processor finds it immediately at start.
 */
__attribute__((section(".isr_vector")))
const VectorTable vector_table = {
    .initial_sp_value = STACK_TOP,      // Step 1: Give the processor a workspace (Stack)
    .reset_handler    = Reset_Handler   // Step 2: Tell it where the code starts
};

/**
 * This is the very first function that runs in my kernel.
 * It functions as a bridge between the raw hardware and C code.
 */
void Reset_Handler(void) {
   
    main();

    // If main against all odds should return, stay here forever.
    while(1);
}


