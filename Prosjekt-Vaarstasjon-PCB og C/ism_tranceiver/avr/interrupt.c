/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "../ism_tranceiver.h"
#include "interrupt.h"

static void pcint0_interrupt(void) {
	
	/* set PCIE0 to enable PCMSK0 scan */
	PCICR |= (1 << PCIE0);
		
	/* set PCINT0 to trigger an interrupt on state change */
	PCMSK0 |= (1 << PCINT0);
	
}


static void init_rtc_interrup(void) {
	
    /* Disable timer2 interrupts */
    TIMSK2  = 0;
	
    /* Enable asynchronous mode */
    ASSR  = (1<<AS2);
	
    /* Set initial counter value */
    TCNT2=0;
	
    /* Set prescaler 128 */
    TCCR2B |= (1<<CS22) | (1<<CS00);
	
    /* Wait for registers update */
    while (ASSR & ((1<<TCN2UB) | (1<<TCR2BUB)));
	
    /* Clear interrupt flags */
    TIFR2  = (1<<TOV2);
	
    /* Enable TOV2 interrupt */
    TIMSK2  = (1<<TOIE2);	
	
}

int init_interrupts(void) {
	
	pcint0_interrupt();
	//init_rtc_interrup();
	
	return EXIT_SUCCESS;
}

