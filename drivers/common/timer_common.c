#include "timer.h"
#include <stddef.h> 


static struct timer *timer_list_head = NULL;


extern volatile uint32_t current_tick;

void timer_setup(struct timer *t, uint32_t ms, void (*cb)(struct timer *t)) {
    t->period = ms;
    t->callback = cb;
    t->enabled = 0U;
    t->next = NULL;
}

void timer_start(struct timer *t) {
    if (t->enabled) return;

    t->expires = current_tick + t->period;
    t->enabled = 1U;
    
    t->next = timer_list_head;
    timer_list_head = t;
}

void timer_stop(struct timer *t) {
    if (!t->enabled) return;

    struct timer *curr = timer_list_head;
    struct timer *prev = NULL;

    while (curr != NULL) {
        if (curr == t) {
            if (prev == NULL) {
                timer_list_head = curr->next;
            } else {
                prev->next = curr->next;
            }
            t->enabled = 0U;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void timer_tick_handler(void) {
    struct timer *t = timer_list_head;
    while (t != NULL) {
        if (t->enabled && (current_tick >= t->expires)) {
            t->callback(t);

            t->expires = current_tick + t->period;
        }
        t = t->next;
    }
}
