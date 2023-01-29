/*******************************************************	
	Description: Timer 1 PWM output on pin PC3s 
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/

#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "timer.h"



int main(void)
{
	delay_init();
	uart_init();
	timer_init();
	uint8_t i = 1;
	
    while(1) {
    	//Set PWM and output mode: NORMAL or INVERTED
		timer_pwm_set(i++, INVERTED);
		i = i % 100;
		delay_ms(10);
    }
}

