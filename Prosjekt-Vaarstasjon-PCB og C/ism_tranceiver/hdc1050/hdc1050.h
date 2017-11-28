/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */


#ifndef HDC1050_H
#define HDC1050_H

int init_hdc1050(void);
int hdc1050_read_temperature(uint8_t *temperature, uint8_t *temperature_decimal);
int hdc1050_read_humidity(uint8_t *humidity);

#define HDC1050_I2C_ADDRESS			0x80

enum {
	HDC1050_REG_TEMPERATURE = 0x00,
	HDC1050_REG_HUMIDITY,
	HDC1050_REG_CONFIGURATION,
	HDC1050_REG_SERIAL_FIRST_TWO_BYTES = 0xFB,
	HDC1050_REG_SERIAL_MID_TWO_BYTES,
	HDC1050_REG_SERIAL_LAST_TWO_BYTES,
	HDC1050_REG_MANUFACTURER_ID,
	HDC1050_REG_DEVICE_ID,
};

#define	HDC1050_MANUFACTURER_ID		0x5449
#define	HDC1050_DEVICE_ID			0x1050

#endif /* HDC1050_H */