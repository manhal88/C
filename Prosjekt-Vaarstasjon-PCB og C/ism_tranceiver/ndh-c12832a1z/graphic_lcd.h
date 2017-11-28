/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#ifndef GRAPHIC_LCD_H
#define GRAPHIC_LCD_H

#define LINE_COUNT			4
#define LINE_PIXEL_LENGHT	128

int graphic_lcd_clear_screen(void);
int graphic_lcd_write(int line, int offset, char *text);
int graphic_lcd_write_rssi(uint8_t rssi);

#endif /* GRAPHIC_LCD_H */