/* 
Function     : Precode for the methods used to interface the LCD on the XMEGA-board
*/

#define F_CPU 32000000
#include "fys3240_led.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//! 14-segment with 4 commons terminals, start segment
#define START_14SEG_4C 12

//Definer disse makroene:
 #define LCD_HOME LCD.CTRLG = LCD_TDG_14S_4C_gc | START_14SEG_4C;
//#define LCD_BACKLIGHT_TOGGLE (PORTE.OUT ^= (1 << 5))

#define LCD_CLEAR LCD.CTRLA |= LCD_CLRDT_bm;
#define LCD_BACKLIGHT_TOGGLE PORTE.OUTSET = BL_PIN;


void lcd_callback_lab2(void);

void lcd_write_char_pos(const uint8_t data, uint8_t pos);
void lcd_write_char(const uint8_t data);
void lcd_write_word(uint8_t *data);
void lcd_write_int(int i);
void init_lcd(void);
void init_lcd_interrupts(void);
