#ifndef STM32F446XX_H
#define STM32F446XX_H

#include <stdint.h>


#define FLASH_BASE      0x08000000UL
#define SRAM_BASE       0x20000000UL

/* Peripherals*/
#define PERIPH_BASE     0x40000000UL
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)


#endif
