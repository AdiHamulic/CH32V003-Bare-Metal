#ifndef __I2C_H
#define __I2C_H

#include "ch32v00x.h"




#define I2C_START()		(I2C1->CTLR1 |= I2C_CTLR1_START)
#define I2C_STOP()		(I2C1->CTLR1 |= I2C_CTLR1_STOP)
#define I2C_ACK()		(I2C1->CTLR1 |= I2C_CTLR1_ACK)
#define I2C_NACK()		(I2C1->CTLR1 &= ~I2C_CTLR1_ACK)

#define I2C_CLEAR_STATUS_REGS()		do { I2C1->STAR1; I2C1->STAR2; } while(0)


typedef enum {
	I2C_STD_MODE  = 100000,
	I2C_FAST_MODE = 400000	
} i2c_speed_t;

typedef enum {
	I2C_WRITE = 0,
	I2C_READ
} I2C_RW_t;


void i2c_init(i2c_speed_t speed);
void i2c_send_addr(uint8_t addr, I2C_RW_t rw);
void i2c_send_data8(uint8_t *data, uint16_t count);
void  i2c_read8(uint8_t *data, uint16_t count);
uint8_t i2c_check_dev(uint8_t addr);


#endif	/* __I2C_H */
