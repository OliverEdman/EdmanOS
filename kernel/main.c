#include "gpio.h"
#include <stddef.h>


void fast_delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++) {
        __asm__("nop");
    }
}

int main(void) {
    
    volatile uint32_t *RCC_AHB1ENR = (uint32_t *)0x40023830;
    *RCC_AHB1ENR |= (1U << 0); 

    struct gpio *led = gpio_new(GPIOA, 10U, GPIO_MODE_OUTPUT);

    
    while(1) {
        if (led != NULL) {
            gpio_toggle(led); 
            fast_delay(1000000U); 
        }
    }
}
