#include <stdint.h>

/* Diagnos-delay för att vi ska hinna se lampan innan stack-testet */
void fast_delay(void) {
    for(volatile int i = 0; i < 500000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    /* --- HÅRDVARU-INIT DIREKT --- */
    volatile uint32_t *RCC_AHB1ENR = (uint32_t *)0x40023830;
    *RCC_AHB1ENR |= (1U << 0); /* Starta klockan för GPIOA */

    volatile uint32_t *GPIOA_MODER = (uint32_t *)0x40020000;
    /* Rensa och sätt pinne 10 till General Purpose Output (01 binärt) */
    *GPIOA_MODER &= ~(3U << (10 * 2));
    *GPIOA_MODER |=  (1U << (10 * 2)); 

    volatile uint32_t *GPIOA_ODR = (uint32_t *)0x40020014;
    
    /* --- TÄND LAMPAN (D2 / PA10) --- */
    *GPIOA_ODR |= (1U << 10); 

    /* Vänta en kort stund med lampan tänd */
    fast_delay();

    /* --- STACK-TEST --- */
    fast_delay();
    *GPIOA_ODR &= ~(1U << 10); /* Släck lampan */

    while(1) {
        /* Blinkar långsamt för succé på D2 */
        *GPIOA_ODR ^= (1U << 10);
        for(volatile int i = 0; i < 2000000; i++) __asm__("nop");
    }
}
