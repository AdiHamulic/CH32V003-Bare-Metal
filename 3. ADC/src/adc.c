/*******************************************************	
	Description: ADC init, cal and read functions
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/


#include "adc.h"
#include "delay.h"






void adc_init(void)
{
	//Enable clock for PORTD and ADC
	RCC->APB2PCENR |= RCC_IOPDEN | RCC_ADC1EN;
	
	//Set GPIOD, pin 4 for analog input mode (CNF and MODE = 0b00)
	GPIOD->CFGLR &= ~(GPIO_CFGLR_CNF4 | GPIO_CFGLR_MODE4);
	
	//Set clock prescaler for ADC - AHB clock 24Mhz / 8 = 3Mhz
	RCC->CFGR0 |= RCC_ADCPRE_4 | RCC_ADCPRE_3;
	
	//Set ADC - External trigger - software trigger mode
	ADC1->CTLR2 =  ADC_EXTTRIG | ADC_EXTSEL_2 | ADC_EXTSEL_1 | ADC_EXTSEL_0;
	
	//Set sample time: (Tconv = Sample time + 11Tadcclk) - 241 ADC cycles for CH7
	ADC1->SAMPTR2 |= ADC_SMP7_2 | ADC_SMP7_1 | ADC_SMP7_0;
	
	//Number of first channel to be read - CH7(0b111) in this chase (PD4)
	ADC1->RSQR3 |= ADC_SQ1_2 | ADC_SQ1_1 | ADC_SQ1_0;
	
	//Enable ADC
	ADC1->CTLR2 |= ADC_ADON;
	delay_ms(1);
	
	//Calibrate ADC
	adc_start_cal();	
}

//Start calibration
void adc_start_cal(void)
{
	//Initialize calibration
	ADC1->CTLR2 |= ADC_RSTCAL;
	//Wait end of init
	while((ADC1->CTLR2 & ADC_RSTCAL) == ADC_RSTCAL){};
	//Start calibration
	ADC1->CTLR2 |= ADC_CAL;
	//Wait end of calibration
	while((ADC1->CTLR2 & ADC_CAL) == ADC_CAL){};
}

uint16_t adc_read(void)
{
	//Start conversation - Software trigger
	ADC1->CTLR2 |= ADC_SWSTART;
	//Wait end of conversion
	while((ADC1->STATR & ADC_EOC) != ADC_EOC){};

	return (uint16_t)ADC1->RDATAR;
}
