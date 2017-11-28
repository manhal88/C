/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "../ism_tranceiver.h"

int spi_init (void) {
	
	/* Enable, Master, and SPI mode 3 */
	SPCR = ( (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA));
	
	/* F_CPU / 2 */
	SPSR = (1<<SPI2X);
	
	return EXIT_SUCCESS;
}

int spi_mode0 (void) {
	
	/* Enable, Master, and SPI mode 0 */
	SPCR = ( (1<<SPE) | (1<<MSTR));
	
	return EXIT_SUCCESS;
}

int spi_mode3 (void) {
	
	/* Enable, Master, and SPI mode 0 */
	SPCR = ( (1<<SPE) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA));
	
	return EXIT_SUCCESS;
}

int spi_transmit (uint8_t *data) {
	
	SPDR = *data;
	while(!(SPSR & (1<<SPIF)));
	
	*data = SPDR;
	
	return EXIT_SUCCESS;
}
