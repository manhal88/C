/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "../ism_tranceiver.h"
#include "graphic_lcd.h"
#include "st7565r.h"
#include "font.h"

int graphic_lcd_clear_screen(void) {
	
	int i, j;
	
	for (i=0; i<LINE_COUNT; i++) {
		st7565r_set_page_address(i);
		st7565r_set_column_address(0);
		st7565r_set_display_start_line_address(0);
		for (j=0; j<LINE_PIXEL_LENGHT; j++)
			st7565r_write_data(0);
	}
	
	return EXIT_SUCCESS;
}

int graphic_lcd_write(int line, int offset, char *text) {

	int i, j;
	
	st7565r_set_page_address(line);
	st7565r_set_column_address(offset);
	
	for (i=0; i<strlen(text); i++) {
	
		for (j=0; j<FONT_SIZE; j++) {
			st7565r_write_data(pgm_read_byte(font_5x8_table + \
			                    (((text[i] - ASCII_FONT_FIRST) * FONT_SIZE) + j) \
							  )); 
		}
		st7565r_write_data(0);
	}
		
	return EXIT_SUCCESS;
}

int graphic_lcd_write_rssi(uint8_t rssi) {
	
	int i;
	uint8_t tmp = 0;
	
	for (i=0; i<8; i++) {
		tmp |= 0x80;
		if (rssi > (25 * i)) {
			tmp >>= 1;
		}
	}
	
	st7565r_write_data(tmp);
	return EXIT_SUCCESS;
}
	
