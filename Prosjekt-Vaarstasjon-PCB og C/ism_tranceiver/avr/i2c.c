/*
 * Copied from https://github.com/g4lvanix/I2C-master-lib
 *
 * MODIFIED BY ELAB
 */

#include "../ism_tranceiver.h"
#include "i2c.h"

static int i2c_start (uint8_t address);
static int i2c_write (uint8_t data);
static int i2c_read_ack (void);
static uint8_t i2c_read_nack (void);
static void i2c_stop(void);

int i2c_init (void) {
	
	/* Minimum value of TWBR is 10 */
	TWBR = 10;	
	return EXIT_SUCCESS;
}

int i2c_write_reg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length) {
	
	if (i2c_start(devaddr | 0x00))
		return EXIT_FAILURE;

	if (i2c_write(regaddr))
		return EXIT_FAILURE;

	for (uint8_t i = 0; i < length; i++) {
		if (i2c_write(data[i])) 
			return EXIT_FAILURE;
	}

	i2c_stop();

	return EXIT_SUCCESS;
}

int i2c_read_reg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length) {
	
	if (i2c_start(devaddr))
		return EXIT_FAILURE;

	if (i2c_write(regaddr))
		return EXIT_FAILURE;

	if (i2c_start(devaddr | 0x01))
		return EXIT_FAILURE;

	for (uint8_t i = 0; i < (length-1); i++) {
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();

	i2c_stop();

	return EXIT_SUCCESS;
}

int i2c_read_reg_10ms_delay(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length) {
	
	if (i2c_start(devaddr))
		return EXIT_FAILURE;

	if (i2c_write(regaddr))
		return EXIT_FAILURE;

	_delay_ms(10);

	if (i2c_start(devaddr | 0x01))
		return EXIT_FAILURE;

	for (uint8_t i = 0; i < (length-1); i++) {
		data[i] = i2c_read_ack();
	}
	data[(length-1)] = i2c_read_nack();

	i2c_stop();

	return EXIT_SUCCESS;
}

static int i2c_start (uint8_t address) {
	
	// reset TWI control register
	TWCR = 0;
	// transmit START condition 
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	
	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
		return EXIT_FAILURE;
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	//_delay_ms(1);
	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

static int i2c_write (uint8_t data) {
	
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}

static int i2c_read_ack (void) {
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	// return received data from TWDR
	return TWDR;
}

static uint8_t i2c_read_nack (void) {
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));
	// return received data from TWDR
	return TWDR;
}

static void i2c_stop(void) {
	
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


