/*******************************************************	
	Description: Read humidity and temperature with 
				 sensor SI7021 over I2C 
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/

#include "ch32v00x.h"
#include "ch32v00x_it.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "i2c.h"
#include "si7021.h"






int main(void)
{
	delay_init();
	uart_init();
	i2c_init(I2C_STD_MODE);
	si7021_measurement_t tmp;
	
    while(1) {
    	if (i2c_check_dev(SI7021_ADDR)){
    		tmp = si7021_read_humidity();
    		//When humidity is measured, we also get temperature measurment
        	printf("Temperature: %d.%d - Humidity: %d\r\n", tmp.temperature / 100, tmp.temperature % 100, tmp.humidity);
        	if (si7021_device_version() == SI7021){
        		printf("*** Sensor is SI7021 ***\r\n");
        	}
    	}
    }
}

