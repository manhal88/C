/* Copyright (c) 2006 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 */

/* MODIFIED BY ELAB */

#include "../ism_tranceiver.h"
#include "../avr/spi.h"

uint8_t hal_nrf_rw(uint8_t value)
{
	spi_mode0();
	uint8_t *data = &value;
	spi_transmit(data);
	return *data;
}
