/**
 * @file gpio.c
 * @brief Implementation of the GPIO driver.
 * @author Oliver Edman <o.edman@icloud.com>
 * @date 2026
 *
 * @note Licensed under the MIT License.
 */

#include "gpio.h"
#include "utils.h"
#include "stm32f446.h"
#include <stddef.h>

/**
 * @struct gpio
 * @brief Internal GPIO instance structure.
 */
struct gpio {
    GPIO_TypeDef *port; /**< Pointer to the GPIO peripheral port */
    uint8_t pin;        /**< Pin number (0-15) */
    bool in_use;        /**< flag for the static pool */
};

/** Static memory pool to avoid heap usage. */
static struct gpio gpio_pool[GPIO_MAX_INSTANCES];

/**
 * @brief Configures raw register settings for a pin.
 * @param port Pointer to the GPIO peripheral.
 * @param pin Pin number (0-15).
 * @param mode Operational mode (INPUT, OUTPUT, etc).
 * @return 0 on success, -1 on invalid arguments.
 */
static int8_t gpio_init(GPIO_TypeDef *port, uint8_t pin, uint32_t mode)
{
    if (port == NULL || pin > 15U) return -1;
    /* Configure 2-bit fields for MODER and PUPDR */
    port->MODER &= ~(0x3UL << (pin * 2U));
    port->MODER |= (mode << (pin * 2U));
    port->PUPDR &= ~(0x3UL << (pin * 2U));
    return 0;
}

/**
 * @brief Allocates and initializes a new GPIO instance from the static pool.
 * @return Pointer to a new gpio instance, or NULL if allocation fails.
 */
struct gpio *gpio_new(GPIO_TypeDef *port, uint8_t pin, uint32_t mode)
{
    if (gpio_init(port, pin, mode) != 0) return NULL;
    for (uint8_t i = 0; i < GPIO_MAX_INSTANCES; i++) {
        if (!gpio_pool[i].in_use) {
            gpio_pool[i].port = port;
            gpio_pool[i].pin = pin;
            gpio_pool[i].in_use = true;
            return &gpio_pool[i];
        }
    }
    return NULL;
}

/**
 * @brief Releases a GPIO instance back to the pool.
 * @param self Double pointer to the instance to free.
 */
void gpio_delete(struct gpio **self)
{
    if (self == NULL || *self == NULL) return;
    (*self)->in_use = false;
    *self = NULL;
}

/**
 * @brief Sets the logical output state of the pin.
 */
int8_t gpio_write(struct gpio *self, bool state)
{
    if (self == NULL || self->port == NULL) return -1;

    if (state) {
        SET(self->port->ODR, self->pin);
    } else {
        CLEAR(self->port->ODR, self->pin);
    }
    
    return 0;
}

/**
 * @brief Reads the current physical input state.
 */
bool gpio_read(const struct gpio *self)
{
    if (self == NULL || self->port == NULL) {
        return false;
    }

    return READ(self->port->IDR, self->pin);
}

/**
 * @brief Toggles the logical output state.
 */
int8_t gpio_toggle(struct gpio *self)
{
    if (self == NULL || self->port == NULL) return -1;
    TOGGLE(self->port->ODR, self->pin);
    return 0;
}
