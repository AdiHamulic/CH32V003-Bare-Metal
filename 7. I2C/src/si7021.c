/*******************************************************	
	Description: SI7021 functions
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/


#include "si7021.h"



void si7021_reset(void)
{
	uint8_t cmd = SI7021_RESET;
	//Start
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	//write CMD data
	i2c_send_data8(&cmd, 1);
	//Stop
	I2C_STOP();
}

uint8_t si7021_read_reg(uint8_t reg)
{
	uint8_t data = 0;
	//Start
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	//write CMD data
	i2c_send_data8((uint8_t *)&reg, 1);
	//ReStart
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	//Read register
	i2c_read8(&data, 1);
	//Stop
	I2C_STOP();
	return data;
}

void si7021_write_reg(uint8_t reg, uint8_t data)
{
	//Start
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	//write CMD data
	i2c_send_data8((uint8_t *)&reg, 1);
	//ReStart
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	//Read register
	i2c_send_data8(&data, 1);
	//Stop
	I2C_STOP();
}

/*
 * Returns firmware version
 * 0xFF older version
 * 0x20 newer version
 */
uint8_t si7021_read_firm_rev(void)
{
	uint8_t cmd[] = { SI7021_FIRMWARE_REV_CMD1, SI7021_FIRMWARE_REV_CMD2 };
	uint8_t data = 0;
	//Start
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	//write CMD data
	i2c_send_data8(cmd, 2);
	//ReStart
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	i2c_read8(&data, 1);
	//Stop
	I2C_STOP();

	return data;
}
/* Return measurement of temperature
 * Temperature:  -10 - 85 degrees (+- 0.4 degrees)
 * Temperature value is integer instead float. Conversation to FLOAT is needed after.
 */

int16_t si7021_read_temp(void)
{
	int16_t raw_temperature;
	uint8_t tmp[2];
	uint8_t cmd = SI7021_READ_TEMP_HD_CMD;
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	i2c_send_data8(&cmd, 1);
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	//Read temperature - MSB first
	i2c_read8(tmp,2);
	raw_temperature = (int16_t)(tmp[0] << 8 | tmp[1]);
	//Integer calculation instead using float. Result eg. 24.52 degrees as 2452
	return (int16_t)(((int32_t)(17572 * raw_temperature) >> 16) - 4685);
}

/*
 * Return measurement of humidity and temperature.
 * Humidity: 0 - 100%
 * Temperature:  -10 - 85 degrees (+- 0.4 degrees)
 * Temperature value is integer instead float. Conversation to FLOAT is needed after.
 */
si7021_measurement_t si7021_read_humidity(void)
{
	int16_t raw_humidity;
	int16_t raw_temperature;
	uint8_t tmp1[2];
	uint8_t tmp2[2];
	uint8_t cmd_h = SI7021_READ_HUMIDITY_HD_CMD;
	uint8_t cmd_t = SI7021_READ_TEMP_AFTER_HUMI;
	si7021_measurement_t value;
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	i2c_send_data8(&cmd_h, 1);
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	//Read humidity - MSB first
	i2c_read8(tmp1,2);
	raw_humidity = (int16_t)(tmp1[0] << 8 | tmp1[1]);
	//Calculate
	value.humidity =  (int8_t)(((uint32_t)(125 * raw_humidity) >> 16) - 6);
	//Limit value:  0 - 100%
	if (value.humidity > 100) value.humidity = 100;
	if (value.humidity < 0) value.humidity = 0;

	//Read temperature after reading humidity
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	i2c_send_data8(&cmd_t, 1);
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	//Read temperature - MSB first
	i2c_read8(tmp2,2);
	raw_temperature = (int16_t)(tmp2[0] << 8 | tmp2[1]);
	//Integer calculation instead using float. Result eg. 24.52 degrees as 2452
	value.temperature =  (int16_t)(((int32_t)(17572 * raw_temperature) >> 16) - 4685);

	return value;

}

/*
 * Set heater ON or OFF
 * Paramters: HEATER_ON or HEATER_OFF
 */

void si7021_heater(si7021_heater_t state)
{
	uint8_t tmp = 0;
	tmp = si7021_read_reg(SI7021_READ_USER_REG1);
	switch (state) {
		case HEATER_ON:
			tmp |= (2 << state);
			si7021_write_reg(SI7021_WRITE_USER_REG1, tmp);
			break;
		case HEATER_OFF:
			tmp &= ~(2 << state);
			si7021_write_reg(SI7021_WRITE_USER_REG1, tmp);
			break;
		default:
			break;
	}
}
/*
 * Set Heater level
 * From 0 - 15 (3mA - 94mA)
 */
void si7021_heater_level(uint8_t level)
{
	uint8_t tmp = 0;
	//First read CTRL register - Recommended by data sheet
	tmp = si7021_read_reg(SI7021_READ_CTRL_REG);
	//Set level
	tmp |= level;
	//Write data back to register
	si7021_write_reg(SI7021_WRITE_CTRL_REG, tmp);
}

/*
 * Return device version from serial number
 * 	0x00 or 0xFF - engineering samples
 * 	0x0D - SI7013
 * 	0x14 - SI7020
 * 	0x15 - SI7021
 */
si7021_device_id si7021_device_version(void)
{
	uint8_t buf[8];
	//First access
	uint8_t cmd[2] = {SI7021_FIRST_ID_1, SI7021_FIRST_ID_2};
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	i2c_send_data8(cmd, 2);
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	i2c_read8(buf,8); //Throw away first read, we only need second read (SNB_3)
	//Second access
	uint8_t cmd2[2] = {SI7021_SECOND_ID_1, SI7021_SECOND_ID_2};
	i2c_send_addr(SI7021_ADDR, I2C_WRITE);
	i2c_send_data8(cmd2, 2);
	i2c_send_addr(SI7021_ADDR, I2C_READ);
	i2c_read8(buf,8);

	return buf[0];
}


