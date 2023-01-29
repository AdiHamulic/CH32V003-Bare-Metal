/**********************************************************	
	Description: Printf over UART DMA
	
	Author: Adamir Hamulic
	Date: 2023

**********************************************************/

#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"




int main(void)
{
	delay_init();
	uart_init();
	
	
    while(1) {
		printf("Hello World!!!\r\n");
		delay_ms(500);
    }
}

