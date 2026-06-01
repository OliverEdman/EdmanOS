#include "gpio.h"
#include <stddef.h>


void fast_delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++) {
        __asm__("nop");
    }
}

int main(void) {
    // starta klocka vväck hårdvara
    volatile uint32_t *RCC_AHB1ENR = (uint32_t *)0x40023830;
    *RCC_AHB1ENR |= (1U << 0); 

    /* Skapa GPIO objekt (D2) */
    struct gpio *led = gpio_new(GPIOA, 10U, GPIO_MODE_OUTPUT);

    if (led != NULL) {
        gpio_write(led, true);  
        fast_delay(500000U);
        
        gpio_write(led, false);
        fast_delay(500000U);
    }

    while(1) {
        if (led != NULL) {
            gpio_toggle(led);
        }
        fast_delay(200000U);
    }
}
