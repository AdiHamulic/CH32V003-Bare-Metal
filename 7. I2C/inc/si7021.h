
#ifndef SI7021_H
#define SI7021_H

#include "ch32v00x.h"
#include "i2c.h"


#define SI7021_ADDR		0x40

#define SI7021_FIRMWARE_REV_CMD1		0x84
#define SI7021_FIRMWARE_REV_CMD2		0xB8
#define SI7021_RESET					0xFE
#define SI7021_READ_TEMP_HD_CMD			0xE3	//Hold master
#define SI7021_READ_TEMP_NOHD_CMD		0xF3	//No hold
#define SI7021_READ_HUMIDITY_HD_CMD		0xE5
#define SI7021_READ_HUMIDITY_NOHD_CMD	0xF5
#define SI7021_READ_TEMP_AFTER_HUMI		0xE0	//Read temperature after humidity
#define SI7021_READ_USER_REG1			0xE7
#define SI7021_WRITE_USER_REG1			0xE6
#define SI7021_READ_CTRL_REG			0x11
#define SI7021_WRITE_CTRL_REG			0x51
#define SI7021_FIRST_ID_1				0xFA
#define SI7021_FIRST_ID_2				0x0F
#define SI7021_SECOND_ID_1				0xFC
#define SI7021_SECOND_ID_2				0xC9




typedef struct {
	uint8_t humidity;
	int16_t temperature;
}si7021_measurement_t;


typedef enum {
	HEATER_OFF = 0x00,
	HEATER_ON = 0x01
}si7021_heater_t;

typedef enum {
	ENGINEERING_SAMPLE1 = 0x00,
	ENGINEERING_SAMPLE2 = 0xFF,
	SI7013 = 0x0D,
	SI7020 = 0x14,
	SI7021 = 0x15
}si7021_device_id;

void si7021_reset(void);
uint8_t si7021_read_reg(uint8_t reg);
void si7021_write_reg(uint8_t reg, uint8_t data);
uint8_t si7021_read_firm_rev(void);
int16_t si7021_read_temp(void);
si7021_measurement_t si7021_read_humidity(void);
void si7021_heater(si7021_heater_t state);
void si7021_heater_level(uint8_t level);
si7021_device_id si7021_device_version(void);




#endif /* SI7021_H */
