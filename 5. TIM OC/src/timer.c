/*******************************************************	
	Description: Timer 1 output compare mode
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/

#include "timer.h"



void timer_init(void)
{
	//Enable clock for TIMER1 and PORTC
	RCC->APB2PCENR |= RCC_TIM1EN | RCC_IOPCEN;
	//Enable PC3(Multiplexed Push-Pull ouput mode) pin - TIM1 channel 3, output at 50Mhz
	GPIOC->CFGLR = (GPIOC->CFGLR & ~(GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3)) \
					| GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_MODE3_0 | GPIO_CFGLR_MODE3_1;
	
	//Disable timer counter
	TIM1->CTLR1 &= ~TIM_CEN;		
	//Configure Timer 1 for output compare mode - T1CH3 at 1kHz
	TIM1->CTLR1 |= TIM_ARPE;
	//Prescaler - 48Mhz / (48000 - 1) = 1kHz
	TIM1->PSC = (uint16_t) 48000 - 1;
	//Auto reload value
	TIM1->ATRLR = (uint16_t)500;
	//Toggle(Flip) output at compare
	TIM1->CHCTLR2 |= TIM_OC3M_1 | TIM_OC3M_0;
	//Enable output
	TIM1->CCER |= TIM_CC3E;
	//Main output enable
	TIM1->BDTR |= TIM_MOE; 
	//Enable timer counter
	TIM1->CTLR1 |= TIM_CEN;
	
}
