#include "cbtimer.h"

#ifndef TIME_MAX
#    define TIME_MAX 0xfffffffful
#endif

uint32_t TickVal(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    uint32_t val = get_heart_tick_time();
    __set_PRIMASK(primask);
    return val;
}

static struct cbtimer head = {.next = NULL, .state = TIMER_STOP};

static struct cbtimer *cbtimer_find(struct cbtimer *p_timer)
{
    struct cbtimer *p = &head;

    while (p->next != NULL) {
        if (p->next == p_timer) {
            return p->next;
        }
        p = p->next;
    }
    return NULL;
}

struct cbtimer *cbtimer_start(struct cbtimer *p_timer, unsigned int timeout, void *param,
                              void (*cb)(void *param))
{
    if (p_timer == NULL) {
        return NULL;
    }

    p_timer->start = TickVal();
    p_timer->timeout = timeout;
    p_timer->param = param;
    p_timer->cb = cb;
    p_timer->state = TIMER_RUN;

    if (cbtimer_find(p_timer) == NULL) {
        p_timer->next = head.next;
        head.next = p_timer;
    }

    return p_timer;
}

void cbtimer_stop(struct cbtimer *p_timer)
{
    if (p_timer == NULL) {
        return;
    }
    p_timer->state = TIMER_STOP;
}

void cbtimer_restart(struct cbtimer *p_timer)
{
    if (p_timer == NULL) {
        return;
    }
    p_timer->state = TIMER_RUN;
    p_timer->start = TickVal();
}

void cbtimer_update(struct cbtimer *p_timer, unsigned int timeout)
{
    if (p_timer == NULL) {
        return;
    }
    p_timer->timeout = timeout;
}

void cbtimer_poll(void)
{
    struct cbtimer *p = head.next;
    uint32_t now_time = TickVal();
    uint32_t interval;

    while (p) {
        if (p->state != TIMER_STOP) {
            if (now_time < p->start) {
                interval = TIME_MAX - (p->start - now_time) + 1;
            } else {
                interval = now_time - p->start + 1;
            }
            if (interval >= p->timeout) {
                p->cb(p->param);
            }
        }
        p = p->next;
    }
}
