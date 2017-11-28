/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "../ism_tranceiver.h"

int uart_init(void) {
	
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0A |= _BV(U2X0);

	/* 8-bit data */ 
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    
	/* Enable RX and TX */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	
	return EXIT_SUCCESS;    
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}
