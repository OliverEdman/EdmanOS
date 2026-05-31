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

#include <stdint.h>
#include <stdbool.h>

/**
 * enum gpio_direction - Defines the direction for a GPIO pin.
 * @GPIO_DIRECTION_INPUT: Input mode without internal pull-up.
 * @GPIO_DIRECTION_INPUT_PULLUP: Input mode with internal pull-up enabled.
 * @GPIO_DIRECTION_OUTPUT: Output mode.
 */
enum gpio_direction {
	GPIO_DIRECTION_INPUT,
	GPIO_DIRECTION_INPUT_PULLUP,
	GPIO_DIRECTION_OUTPUT,
};

struct gpio;

/**
 * gpio_new() - Creates a new GPIO instance.
 * @pin: Pin number on the MCU.
 * @direction: Desired direction (see &enum gpio_direction).
 * @return: A pointer to a newly allocated &struct gpio instance, or NULL on failure.
 */
struct gpio *gpio_new(uint8_t pin, enum gpio_direction direction);

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
int gpio_write(struct gpio *self, bool state);

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
int gpio_toggle(struct gpio *self);
