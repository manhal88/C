/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "../ism_tranceiver.h"
#include "hdc1050.h"
#include "../avr/i2c.h"

int init_hdc1050(void) {
	
	uint8_t data[2];
	
	/* Verify device ID */
	if (i2c_read_reg(HDC1050_I2C_ADDRESS, HDC1050_REG_DEVICE_ID, data, 2))
		return EXIT_FAILURE;
		
	if (((data[0] << 8) | data[1])  != HDC1050_DEVICE_ID)
		return EXIT_FAILURE;
		
	/* Configuration:
	 * 
	 * Heater enabled, Temperature or humidity is acuired,
	 * Battery voltage grater than 2V8, 14-bit resolution,
	 * 
	 */
	data[0] = 0x00;
	data[1] = 0x00;
	
	if (i2c_write_reg(HDC1050_I2C_ADDRESS, HDC1050_REG_CONFIGURATION, data, 2))
		return EXIT_FAILURE;	
	
	return EXIT_SUCCESS;
}

int hdc1050_read_temperature(uint8_t *temperature, uint8_t *temperature_decimal) {

	uint8_t data[2];
	float temp;
	
	if (i2c_read_reg_10ms_delay(HDC1050_I2C_ADDRESS, HDC1050_REG_TEMPERATURE, data, 2))
		return EXIT_FAILURE;
		
	temp = ((data[0] << 8) | data[1]) * 165UL;
	temp /= 0xffff;
	temp -= 40;
	
	*temperature = (uint8_t)temp;
	*temperature_decimal = (uint8_t)(temp * 10);

	return EXIT_SUCCESS;
}

int hdc1050_read_humidity(uint8_t *humidity) {
	
	uint8_t data[2];
	float hum;
	
	if (i2c_read_reg_10ms_delay(HDC1050_I2C_ADDRESS, HDC1050_REG_HUMIDITY, data, 2))
		return EXIT_FAILURE;
	
	hum = ((data[0] << 8) | data[1]) * 100UL;
	hum /= 0xffff;
	
	*humidity = (uint8_t)hum;
	
	return EXIT_SUCCESS;
}