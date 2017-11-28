/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "ism_tranceiver.h"

volatile bool second_tick = false;

ISR (REAL_TIME_SECOND_IRQ_VECTOR) {

	second_tick = true;
	puts("tick, tack");	
		
}