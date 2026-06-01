/**
 * @file gpio.h
 * @brief interface for the gpio driver.
 * @author Oliver Edman <o.edman@icloud.com>
 * @date 2026
 *
 * @note Licensed under the MIT License (see LICENSE file in root).
 * Copyright (c) 2026 Oliver Edman
 */

#pragma once

#include "stm32f446.h"

#include <stdint.h>
#include <stdbool.h>

#define GPIO_MAX_INSTANCES 16U 

#define GPIO_MODE_INPUT 0x0UL
#define GPIO_MODE_OUTPUT 0x1UL
#define GPIO_MODE_ALT 0x2UL
#define GPIO_MODE_ANALOG 0x3UL

#define GPIO_PUPD_NONE 0x0UL
#define GPIO_PUPD_UP 0x1UL
#define GPIO_PUPD_DOWN 0x2UL


struct gpio;

/**
 * gpio_new() - Creates a new GPIO instance.
 * @pin: Pin number on the MCU.
 * @direction: Desired direction (see &enum gpio_direction).
 * @return: A pointer to a newly allocated &struct gpio instance, or NULL on failure.
 */
struct gpio *gpio_new(GPIO_TypeDef *port, uint8_t pin, uint32_t mode);

/**
 * gpio_delete() - Frees a GPIO instance and nullifies the pointer.
 * @self: Double pointer to the GPIO object to be freed.
 *
 * Sets the pointer to NULL after deallocation to prevent dangling pointers.
 */
void gpio_delete(struct gpio **self);

/**
 * gpio_write() - Sets the state of a GPIO pin.
 * @self: Pointer to the GPIO object.
 * @state: Boolean value (true for high, false for low).
 */
int8_t gpio_write(struct gpio *self, bool state);

/**
 * gpio_read() - Reads the state of a GPIO pin.
 * @self: Pointer to the GPIO object.
 * @return: true if the pin is high, false if the pin is low.
 * Note: If self is NULL, default behavior is false.
 */
bool gpio_read(const struct gpio *self);

/**
 * gpio_toggle() - Toggles the state of a GPIO pin.
 * @self: Pointer to the GPIO object.
 * @return: 0 on success, negative error code on failure.
 */
int8_t gpio_toggle(struct gpio *self);
