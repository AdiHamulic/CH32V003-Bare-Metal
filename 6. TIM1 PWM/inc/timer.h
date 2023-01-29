#ifndef __TIMER_H
#define __TIMER_H

#include "ch32v00x.h"

//Output polarity
#define NORMAL		0
#define INVERTED	1


void timer_init(void);
void timer_pwm_set(uint16_t pwm, uint8_t set);




#endif	/* __TIMER_H */
