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
    *GPIOA_MODER &= ~(3U << (5 * 2));
    *GPIOA_MODER |=  (1U << (5 * 2)); /* PA5 till Output */

    volatile uint32_t *GPIOA_ODR = (uint32_t *)0x40020014;
    
    /* --- TÄND LAMPAN --- */
    /* Vi tänder lampan med rå hårdvaru-access.
       Om lampan lyser till grönt nu vet vi att vi nådde hit. */
    *GPIOA_ODR |= (1U << 5); 

    /* Vänta en kort stund med lampan tänd för att vi ska hinna se den */
    fast_delay();

    /* --- STACK-TEST --- */
    /* Om stackpekaren pekar fel, kraschar processorn precis HÄR när 
       vi försöker spara returadressen för funktionen fast_delay().
       Om lampan släcks, lyckades hoppet! */
    fast_delay();
    *GPIOA_ODR &= ~(1U << 5); /* Släck lampan */

    while(1) {
        /* Om vi lyckades med stack-testet och släkte lampan,
           blinkar vi långsamt för succé. */
        *GPIOA_ODR ^= (1U << 5);
        for(volatile int i = 0; i < 2000000; i++) __asm__("nop");
    }
}
