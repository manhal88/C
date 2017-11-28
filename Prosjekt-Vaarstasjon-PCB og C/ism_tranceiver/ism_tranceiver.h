/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#ifndef ISM_TRANCEIVER
#define ISM_TRANCEIVER

#include <inttypes.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU						(1000000UL)
#define BAUD						(9600)

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/twi.h>
#include <util/delay.h>
#include <util/setbaud.h>

#define I_SET(type,name,bit)		type ## name  |= _BV(bit)    
#define I_CLEAR(type,name,bit)		type ## name  &= ~ _BV(bit)        
#define I_TOGGLE(type,name,bit)		type ## name  ^= _BV(bit)    
#define I_GET(type,name,bit)		((type ## name >> bit) &  1)

#define OUTPUT(pin)					I_SET(DDR,pin)    
#define INPUT(pin)					I_CLEAR(DDR,pin)    
#define HIGH(pin)					I_SET(PORT,pin)
#define LOW(pin)					I_CLEAR(PORT,pin)    
#define TOGGLE(pin)					I_TOGGLE(PORT,pin)    
#define READ(pin)					I_GET(PIN,pin)

#define REAL_TIME_SECOND_IRQ_VECTOR	TIMER2_OVF_vect

/* Port B */
#define NRF24L01_IRQ				B,0
#define NRF24L01_IRQ_VECTOR			PCINT0_vect

#define NRF24L01_CHIP_SELECT		B,1
#define NRF24L01_CHIP_ENABLE		B,2
	
#define SPI_MOSI					B,3
#define SPI_MISO					B,4
#define	SPI_SCK						B,5

/* Port C */
#define	LCD_A0						C,0
#define	LCD_RESET					C,1
#define	LCD_CHIP_SELECT				C,2

/* Port D */
#define DOOR_BELL_SWITCH			D,2
				
#define LED_ORANGE					D,3
#define LED_ORANGE_ON()				do { HIGH(LED_ORANGE);			} while (0)
#define LED_ORANGE_OFF()			do { LOW(LED_ORANGE);			} while (0)
#define LED_ORANGE_TOGGLE()			do { TOGGLE(LED_ORANGE);		} while (0)

#define LED_BLUE					D,4
#define LED_BLUE_ON()				do { HIGH(LED_BLUE);			} while (0)
#define LED_BLUE_OFF()				do { LOW(LED_BLUE);				} while (0)
#define LED_BLUE_TOGGLE()			do { TOGGLE(LED_BLUE);			} while (0)

#define	LCD_BACKLIGHT				D,6
#define LCD_BACKLIGHT_ON()			do { HIGH(LCD_BACKLIGHT);		} while (0)
#define LCD_BACKLIGHT_OFF()			do { LOW(LCD_BACKLIGHT);		} while (0)

int init_hardware(void);

#endif /* ISM_TRANCEIVER */