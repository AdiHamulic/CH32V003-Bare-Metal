/**********************************************************	
	Description: Returns entered character over terminal
	
	Author: Adamir Hamulic
	Date: 2023

**********************************************************/



#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"
#include "uart.h"


int main(void)
{

	delay_init();
	uart_init();
	uart_send_str("CH32V003 Hello World!!!\n\r");
	char temp;
    while(1) {
    	temp = uart_recv_ch();
    	if(temp){
    		uart_send_str("Entered char: ");
    		uart_send_ch(temp);
    		uart_send_str("\r\n");
    	}
    }
}

