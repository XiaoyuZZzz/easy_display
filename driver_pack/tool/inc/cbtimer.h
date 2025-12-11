#ifndef CBTIMER_H__
#define CBTIMER_H__

#include "time_tick.h"

#include "stdint.h"
#include "stdio.h"

typedef enum{
	TIMER_STOP,
	TIMER_RUN,
	TIMER_RESTART
}Timer_State;

struct cbtimer{
	struct cbtimer *next;
	uint32_t start;
	uint32_t timeout;
	void *param;
	void(*cb)(void *param);
	unsigned short state;
};

struct cbtimer* cbtimer_start(struct cbtimer *p_timer,unsigned int timeout,void *param,void(*cb)(void *param));
void cbtimer_stop(struct cbtimer *p_timer);
void cbtimer_restart(struct cbtimer *p_timer);
void cbtimer_update(struct cbtimer *p_timer,unsigned int timeout);
void cbtimer_poll(void);

/**TIMER_API*/
uint32_t TickVal(void);

#endif
