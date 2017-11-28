/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "ism_tranceiver.h"
#include "avr/uart.h"
#include "avr/spi.h"
#include "avr/i2c.h"
#include "avr/interrupt.h"
#include "ndh-c12832a1z/st7565r.h"
#include "hdc1050/hdc1050.h"

int init_hardware(void) {
	
	/* Wait for power to stabilize */
	_delay_ms(100);
	
	/* Indicator LED's */
	OUTPUT(LED_ORANGE);
	OUTPUT(LED_BLUE);
	
	/* Switch */
	INPUT(DOOR_BELL_SWITCH);

	/* Radio connections */
	INPUT(NRF24L01_IRQ);
	OUTPUT(NRF24L01_CHIP_ENABLE);
	LOW(NRF24L01_CHIP_ENABLE);
	OUTPUT(NRF24L01_CHIP_SELECT);
	HIGH(NRF24L01_CHIP_SELECT);
	
	/* SPI initialization */
	INPUT(SPI_MISO);
	OUTPUT(SPI_MOSI);
	OUTPUT(SPI_SCK);	
	
	if (spi_init())
		return EXIT_FAILURE;
	
	/* Redirecting STDIN and STDOUT to UART */
	if (uart_init())
		return EXIT_FAILURE;
	stdout = &uart_output;
	stdin = &uart_input;
	
	/* I2C initialization */
	if (i2c_init())
		return EXIT_FAILURE;
	
	if (init_hdc1050())
		return EXIT_FAILURE;
	
	/* Set up interrupts */
	if (init_interrupts())
		return EXIT_FAILURE;
		
	return EXIT_SUCCESS;
}
	
