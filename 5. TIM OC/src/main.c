/***********************************************************	
	Description: Toggle output TIM1 channel 3 on pin PC3
	
	Author: Adamir Hamulic
	Date: 2023

***********************************************************/

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
	
	
    while(1) {
		printf("Blinky -> Timer1 -> OC3\r\n");
		delay_ms(1000);
    }
}

