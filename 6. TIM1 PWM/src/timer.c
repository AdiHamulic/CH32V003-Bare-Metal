/*******************************************************	
	Description: Timer1 PWM mode channel 1 on PC3
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/

#include "timer.h"



void timer_init(void)
{
	//Enable clock for TIMER1 and PORTC
	RCC->APB2PCENR |= RCC_TIM1EN | RCC_IOPCEN;
	//Enable PC3(Multiplexed Push-Pull ouput mode) pin - TIM1 channel 1, output at 50Mhz
	GPIOC->CFGLR = (GPIOC->CFGLR & ~(GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3)) \
					| GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_MODE3_0 | GPIO_CFGLR_MODE3_1;
	
	//Disable timer counter
	TIM1->CTLR1 &= ~TIM_CEN;		
	//Configure Timer 1 for output compare mode - T1CH3 at 10kHz
	TIM1->CTLR1 |= TIM_ARPE;
	//Prescaler - 48Mhz / (4800 - 1) = 10kHz
	TIM1->PSC = (uint16_t) 4800 - 1;
	//Auto reload value
	TIM1->ATRLR = (uint16_t)100;
	//Percentage - 50%
	TIM1->CH3CVR = (uint16_t)50;
	//PWM mode 1
	TIM1->CHCTLR2 |= TIM_OC3M_2 | TIM_OC3M_1;
	//Enable output
	TIM1->CCER |= TIM_CC3E;
	//Main output enable
	TIM1->BDTR |= TIM_MOE; 
	//Enable timer counter
	TIM1->CTLR1 |= TIM_CEN;
	
}

void timer_pwm_set(uint16_t pwm, uint8_t set)
{
	uint16_t period = TIM1->ATRLR;
	//Set output polarity
	if (!set) TIM1->CCER &= ~TIM_CC3P;
	else TIM1->CCER |= TIM_CC3P;
	//Set PWM width
	TIM1->CH3CVR = (uint16_t)((period * pwm) / 100);
}

