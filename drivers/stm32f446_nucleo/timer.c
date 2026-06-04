#include "timer.h"
#include <stdint.h>
#include <stddef.h>

static struct timer timer_pool[MAX_TIMERS];
static struct timer *timer_list_head = NULL;
static volatile uint32_t system_ticks = 0;

int timer_setup(void)
{
	return 0;	
}

int timer_start(struct timer *t)
{
	if (t == NULL) return -1; // add -EINVAL later
	
	t->expires = timer_get_ticks() + t->period;

	t->next = timer_list_head;
	timer_list_head = t;
	t->enabled = true;
	return 0; 
}


int timer_stop(struct timer *t)
{
    if (t == NULL) return -1;

    struct timer *curr = timer_list_head;
    struct timer *prev = NULL;

    while (curr != NULL) {
        if (curr == t) {
            if (prev == NULL) {
                
                timer_list_head = curr->next;
            } else {
        
                prev->next = curr->next;
            }
            curr->enabled = false;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}

uint32_t timer_get_ticks(void) 
{
	return system_ticks;
}

int timer_tick_handler(void)
{
    struct timer *curr = timer_list_head;
    struct timer *prev = NULL;
    uint32_t now = timer_get_ticks();

    while (curr != NULL) {
        if (curr->enabled && (now >= curr->expires)) {
            
            if (curr->callback) curr->callback(curr);

            
            if (curr->period > 0) {
                curr->expires = now + curr->period;
                prev = curr;
                curr = curr->next;
            } else {
                
                struct timer *next = curr->next;
                if (prev == NULL) timer_list_head = next;
                else prev->next = next;
                
                curr->enabled = false;
                curr = next; 
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return 0;
}

struct timer* timer_allocate(void) 
{
    for (int i = 0; i < MAX_TIMERS; i++) {
        
        if (!timer_pool[i].enabled) {
            
            timer_pool[i].expires = 0;
            timer_pool[i].period = 0;
            timer_pool[i].callback = NULL;
            timer_pool[i].next = NULL;
            timer_pool[i].enabled = true; 
            return &timer_pool[i];
        }
    }
    return NULL; 
}
