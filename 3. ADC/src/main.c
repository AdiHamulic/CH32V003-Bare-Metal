/*******************************************************	
	Description: Read ADC value from channel 7 - pin D4
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/
#include "stdio.h"
#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"




int main(void)
{
	delay_init();
	uart_init();
	adc_init();

	
	
    while(1) {
		printf("ADC value: %d\r\n", adc_read());
		delay_ms(10);
		
    }
}

