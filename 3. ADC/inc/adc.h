#ifndef __ADC_H
#define __ADC_H

#include "ch32v00x.h"


void adc_init(void);
void adc_start_cal(void);
uint16_t adc_read(void);






#endif	/* __ADC_H */
