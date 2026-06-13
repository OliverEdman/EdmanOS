/**
 * @file timer.h
 * @brief an simple timer driver interface.
 * This module provides a flexible way to manage multiple software timers.
 * Each timer is an independent object that carries its own data and callback logic.
 * @autor Oliver Edman <o.edman@icloud.com>
 * @date 2026
 * 
 * @note Licensed under MIT License (see License file in root).
 * Copyright (c) 2026 Oliver Edman
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>


/**
 * @struct timer
 
 * @brief Represents a software timer instance. 
 */
struct timer {
	/** @brief Absolute tick count when this timer triggers. */
	uint32_t expires; 

	/** @brief Period in ticks.*/
	uint32_t period;  

	/** @brief Function to call when timer expires. */
	void (*callback)(struct timer *t); 

	/** @brief True if the timer is currently running. */
	bool enabled;     

	/** @brief Pointer to the next timer in the linked list. */
	struct timer *next; 
};

// this number can be changed later when i know how many timers will be needed.
#define MAX_TIMERS 10 


/**
 * @brief Initialize the timer hardware.
 * @return 0 on success, negative error code on failure.
 */
int timer_setup(void);

/**
 * @brief Get the current system time in milliseconds.
 * @return Current tick count.
 */
uint32_t timer_get_ticks(void);

/**
 * @brief Add the timer to the active timer list.
 * @param t Pointer to the timer instance.
 * @return 0 on success, -EINVAL if pointer is NULL, -EBUSY if already running.
 */
int timer_start(struct timer *t);

/**
 * @brief Remove the timer from the active timer list.
 * @param Pointer to a timer struct.
 * @return 0 on success, -EINVAL if pointer is NULL or timer not found.
 */
int timer_stop(struct timer *t);

/**
 * @brief Handle the periodic tick interrupt.
 * @return 0 on success.
 */
int timer_tick_handler(void);

struct timer* timer_allocate(void);
