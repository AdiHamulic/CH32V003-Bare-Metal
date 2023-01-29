/*******************************************************	
	Description: UART functions
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/


#include "uart.h"
#include "string.h"

// If set high, transmition in progress
volatile uint8_t uart_dma_tc_flag = 0;



void uart_init(void)
{
	//Enable clock for PORTD, USART1 and DMA1
	RCC->APB2PCENR |=  RCC_IOPDEN | RCC_USART1EN;
	RCC->AHBPCENR |= RCC_DMA1EN;
	
	//Enable PD6(RX) and PD5(TX) - PD6(Floating input), PD5(Multiplexed Push-Pull ouput mode 50Mhz)
	GPIOD->CFGLR = (GPIOD->CFGLR & ~(GPIO_CFGLR_CNF6 | GPIO_CFGLR_MODE5 | GPIO_CFGLR_MODE6)) \
					| GPIO_CFGLR_CNF6_0 | GPIO_CFGLR_CNF5_1 | GPIO_CFGLR_MODE5_0 | GPIO_CFGLR_MODE5_1;
					
	//Set DMA Channel 4 - Priority very high, memory increment, memory -> peripheral, enable TC interrupt
	DMA1_Channel4->CFGR |= DMA_CFGR1_PL_0 | DMA_CFGR1_PL_1 | DMA_CFGR1_MINC | DMA_CFGR1_DIR | DMA_CFGR1_TCIE;
	DMA1_Channel4->PADDR = (uint32_t)&USART1->DATAR;
	
	//Enable DMA Channel 4 interrupt
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	
	//Enable TX and RX 
	USART1->CTLR1 = USART_CTLR1_RE | USART_CTLR1_TE;
	
	//Enable DMA for TX
	USART1->CTLR3 |= USART_CTLR3_DMAT;
	
	//UART1 Baud rate - 115200
	USART1->BRR = (((uint16_t)UART_BAUD_115200) << 4) & USART_BRR_DIV_Mantissa;
	USART1->BRR |= (((uint16_t)UART_BAUD_115200) >> 4) & USART_BRR_DIV_Fraction;
	
	//Enable USART1
	USART1->CTLR1 |= USART_CTLR1_UE;
}

void uart_send_ch(char data)
{
	while((USART1->STATR & USART_STATR_TC) != USART_STATR_TC) {};
	USART1->DATAR = data;	
}
void uart_send_str(char *data)
{	
	while(*data) {
		uart_send_ch(*data++);
	}
}

char uart_recv_ch(void)
{
	if ((USART1->STATR & USART_STATR_RXNE) == USART_STATR_RXNE) {
		return (char)(USART1->DATAR);
	}
	return 0;
}

void uart_send_str_dma(char *str)
{
	//Wait end of transmition
	while(uart_dma_tc_flag){};
	//Disable DMA Channel
	DMA1_Channel4->CFGR &= ~DMA_CFGR1_EN;
	//Set buffer address
	DMA1_Channel4->MADDR = (uint32_t)str;
	//Number of data to be send
	DMA1_Channel4->CNTR = (uint16_t)strlen(str);
	//Enable DMA Channel
	DMA1_Channel4->CFGR |= DMA_CFGR1_EN;
	uart_dma_tc_flag = 1;		
}

void uart_send_data_dma(char *data, uint16_t size)
{
	//Wait end of transmition
	while(uart_dma_tc_flag){};
	//Disable DMA Channel
	DMA1_Channel4->CFGR &= ~DMA_CFGR1_EN;
	//Set buffer address
	DMA1_Channel4->MADDR = (uint32_t)data;
	//Number of data to be send
	DMA1_Channel4->CNTR = (uint16_t)size;
	//Enable DMA Channel
	DMA1_Channel4->CFGR |= DMA_CFGR1_EN;
	uart_dma_tc_flag = 1;		
}

