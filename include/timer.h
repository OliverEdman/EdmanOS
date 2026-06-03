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
 * @brief Represents a single timer instance.
 */
struct timer {
    uint32_t expires;
    uint32_t period;
    void (*callback)(struct timer *t);
    bool enabled;
    struct timer *next;
};

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
 * @param t Pointer to the timer instance.
 * @return 0 on success, -EINVAL if pointer is NULL or timer not found.
 */
int timer_stop(struct timer *t);

/**
 * @brief Handle the periodic tick interrupt.
 * @return 0 on success.
 */
int timer_tick_handler(void);
