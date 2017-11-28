/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */


#ifndef SPI_H
#define SPI_H

int spi_init (void);
int spi_mode0 (void);
int spi_mode3 (void);
int spi_transmit (uint8_t *data);

#endif /* SPI_H */