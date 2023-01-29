/*******************************************************	
	Description: Connect LED1 or LED2 to pin D0
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/


#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"



int main(void)
{

	delay_init();

	//Enable clock for Port D
	RCC->APB2PCENR |= RCC_IOPDEN;
	//Set PD0 as output - low speed 
	GPIOD->CFGLR = (GPIOD->CFGLR & ~GPIO_CFGLR_MODE0) | GPIO_CFGLR_MODE0_0; 

    while(1) {
    	GPIOD->BSHR = GPIO_BSHR_BR0;
    	delay_ms(500);
    	GPIOD->BSHR = GPIO_BSHR_BS0;
    	delay_ms(500);
    }
}

