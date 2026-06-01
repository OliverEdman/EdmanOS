/**
 * @file timer.h
 * @brief A simple linked list based timer driver interface.
 * * This module provides a flexible way to manage multiple software timers.
 * Each timer is an independent object that carries its own data and callback logic.
 * @autor Oliver Edman <o.edman@icloud.com>
 * @date 2026
 * 
 * @note Licensed under MIT License (see License file in root).
 * Copyright (c) 2026 Oliver Edman
 *
 */



#pragma once

#include <stdint.h>


extern volatile uint32_t current_tick;

/**
 * @struct timer
 * @brief Represents a single timer instance.
 */
struct timer {
	uint32_t expires; /**< The tick value when this timer triggers. */
	uint32_t period; /**< The interval in milliseconds for periodic timers. */
	void (*callback)(struct timer *t); /**< Pointer to the callback function. */
	int enabled; /**< Non zero if the timer is active. */
	struct timer *next;  /**< Pointer to the next timer in the linked list. */
};

/**
 * @brief Initializes a timer object.
 * * Sets the default values and links the callback function. 
 * The timer is set to disabled by default.
 * * @param t Pointer to the timer instance to initialize.
 * @param ms The duration in milliseconds.
 * @param cb Pointer to the callback function.
 */
void timer_setup(struct timer *t, uint32_t ms, void (*cb)(struct timer *t));

/**
 * @brief Adds the timer to the active timer list.
 * * @param t Pointer to the timer instance to start.
 */
void timer_start(struct timer *t);

/**
 * @brief Removes the timer from the active timer list and disables it.
 * * @param Pointer to the timer instance to stop.
 */
void timer_stop(struct timer *t);

void timer_tick_handler(void);

