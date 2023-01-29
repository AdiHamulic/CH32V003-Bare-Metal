/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main Interrupt Service Routines.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include <ch32v00x_it.h>



extern volatile uint32_t delay_cnt; 
extern volatile uint8_t uart_dma_tc_flag;


/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}
/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   This function is for Systick timer.
 *
 * @return  none
 */
void SysTick_Handler(void)
{
	SysTick->SR = 0;
	if (delay_cnt > 0) delay_cnt--;
}

/*********************************************************************
 * @fn      DMA1 Channel 4 Handler
 *
 * @brief   This function handles DMA.
 *
 * @return  none
 */
void DMA1_Channel4_IRQHandler(void)
{
	if (((DMA1->INTFR & DMA_TCIF4) == DMA_TCIF4)){
		//Clear TC flag
		DMA1->INTFCR |= DMA_CTCIF4;
		uart_dma_tc_flag = 0;
	} else {
		//Clear Global flag
		DMA1->INTFCR |= DMA_CGIF4;
	}

}

