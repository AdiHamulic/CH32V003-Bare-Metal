/*******************************************************	
	Description: I2C functions
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/



#include "i2c.h"





static void i2c_wait(uint16_t status)
{
	while((I2C1->STAR1 & status) != status){
		//Add timeout
	}
}

void i2c_init(i2c_speed_t speed)
{
	uint32_t apb1_clk;
	uint32_t tmp;

	//Set APB1 clock to 24Mhz if SYSCLK is 48Mhz
	//RCC->CFGR0 |= RCC_PPRE1_0;

	//Get APB1 clock
	if (!(RCC->CFGR0 & RCC_PPRE1)) {
		//APB1 == SYSCLK(48Mhz)
		apb1_clk = SystemCoreClock;
	} else {
		tmp = RCC->CFGR0;
		tmp = (tmp >> 8) & 0x00000007;
		apb1_clk = SystemCoreClock / (tmp << 1);
	}

	//Enable RCC for I2C and PORTC
	RCC->APB1PCENR |= RCC_I2C1EN;
	RCC->APB2PCENR |=  RCC_IOPCEN;
	//Enable RCC for DMA
	RCC->AHBPCENR |= RCC_DMA1EN;

	//Enable PC1(SDA) and PC2(SCL) ports - Open-drain multiplexed output - Max speed 50Mhz
	GPIOC->CFGLR = (GPIOC->CFGLR & ~(GPIO_CFGLR_CNF2 | GPIO_CFGLR_CNF1 \
				  | GPIO_CFGLR_MODE2 | GPIO_CFGLR_MODE1)) | GPIO_CFGLR_CNF2_1 \
				  | GPIO_CFGLR_CNF2_0 | GPIO_CFGLR_CNF1_1 | GPIO_CFGLR_CNF1_0 \
				  | GPIO_CFGLR_MODE2_1 | GPIO_CFGLR_MODE2_0 | GPIO_CFGLR_MODE1_1 \
				  | GPIO_CFGLR_MODE1_0;
	
	//First disable I2C 
	I2C1->CTLR1 &= ~I2C_CTLR1_PE;
	//Enable I2C DMA
	I2C1->CTLR2 |= I2C_CTLR2_DMAEN;
	//Configure DMA (Channel 6 -> I2C TX), High priority, memory increment
	//Direction - memory to peripheral
	DMA1_Channel6->CFGR |= DMA_CFGR1_PL_1 | DMA_CFGR1_MINC | DMA_CFGR1_DIR;
	// Set peripheral address - I2C1 data register
	DMA1_Channel6->PADDR = (uint32_t)&I2C1->DATAR;


	//Set I2C bus frequency - APB1
	I2C1->CTLR2 |= (uint16_t)(apb1_clk / 1000000);
	
	//Set I2C speed
	if (speed == I2C_STD_MODE) {
		//Standard mode - 100kHz
		I2C1->CKCFGR = (uint16_t)(apb1_clk / (I2C_STD_MODE * 2));
	} else { //Fast mode - 400 kHz
		//Set Fast mode and duty = 33.3%(2)
		if (speed == I2C_FAST_MODE) {
			I2C1->CKCFGR = (I2C1->CKCFGR & ~I2C_CKCFGR_DUTY) | I2C_CKCFGR_FS;
		}
		//Check is duty Tlow / Thigh = 16/9(36%)
		if ((I2C1->CKCFGR & I2C_CKCFGR_DUTY) == I2C_CKCFGR_DUTY) {
			I2C1->CKCFGR = (uint16_t)(apb1_clk / (I2C_FAST_MODE * 25));
		} else { // Duty is Tlow / Thigh = 2 (33.3%)	
			I2C1->CKCFGR = (uint16_t)(apb1_clk / (I2C_FAST_MODE * 3));
		}		
	}
	//Enable I2C
	I2C1->CTLR1 |= I2C_CTLR1_PE;

	I2C_ACK();
}

void i2c_send_addr(uint8_t addr, I2C_RW_t rw)
{
	I2C_START();
	//Wait master mode or start bit
	i2c_wait(I2C_STAR1_SB);
	//Write address + read data
	I2C1->DATAR = (addr << 1) | rw;
	//Wait address ACK
	i2c_wait(I2C_STAR1_ADDR);
	//Clear
	I2C_CLEAR_STATUS_REGS();
}

void i2c_send_data8(uint8_t *data, uint16_t count)
{
	for (uint16_t i = 0; i < count; i++){
		i2c_wait(I2C_STAR1_TXE);
		//write data
		I2C1->DATAR = data[i];
		//Wait transfer to finish
		i2c_wait(I2C_STAR1_BTF);
		//Clear
		I2C_CLEAR_STATUS_REGS();
	}
}

void  i2c_read8(uint8_t *data, uint16_t count)
{
	uint16_t i = 0;
	do {
		//Wait for data
		i2c_wait(I2C_STAR1_RXNE);
		//Clear
		I2C_CLEAR_STATUS_REGS();
		//Read
		data[i++] = I2C1->DATAR;
		//Send STOP and clear NACK, if it's end
		if (i == (count - 1)) {
			I2C_STOP();
		}
	} while(i < count);
}

uint8_t i2c_check_dev(uint8_t addr)
{
	uint8_t status = 0;
	I2C_START();
	//Wait master mode or start bit
	i2c_wait(I2C_STAR1_SB);
	//Write address + read data
	I2C1->DATAR = (addr << 1) | I2C_READ;
	//Wait address ACK
	i2c_wait(I2C_STAR1_ADDR);
	//Check address ACk
	if((I2C1->STAR1 & I2C_STAR1_ADDR) == I2C_STAR1_ADDR){
		status = 1;
	}
	//Clear
	I2C_CLEAR_STATUS_REGS();
	I2C_STOP();

	return status;
}

