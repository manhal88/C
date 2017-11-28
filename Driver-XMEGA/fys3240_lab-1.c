/* 
Function     : Precode for a microcontrollerprogram interfacing the LCD
*/

#include <util/delay.h>
#include "fys3240_lcd.h"
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t counter = 0;

int main() {
	init_lcd();
	init_buttons();
	PORTE.DIR = 0x20;
	PORTE.OUTSET = 0x20;
	TestApp();
	while(1){
	}
}

void TestApp() {
	
	// test1: writing a Char to LCD
	uint8_t word1[] = "test 1";
	lcd_write_word(word1);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	uint8_t data1 = 'F';
	lcd_write_char(data1);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	
	
	// test2: Writing a char in certain position LCD
	uint8_t word2[] = "test 2";
	lcd_write_word(word2);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	uint8_t data2 = 'Y';
	uint8_t pos1 = 1;
	lcd_write_char_pos(data2,pos1);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	
	//test3: write a char to LCD and enables blinking at 2Hz ration
	uint8_t word3[] = "test 3";
	lcd_write_word(word3);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	uint8_t blink_rate1 = 3;
	uint8_t data3 = 'S';
	uint8_t pos2 = 2;
	lcd_write_char_pos(data3,pos2);
	lcd_set_blink(blink_rate1);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;
	
	//test4 writes an Int to LCD
	lcd_blink_disable();
	uint8_t word4[] = "test 4";
	lcd_write_word(word4);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;	
	int number = 3;
	lcd_write_int(number);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;

	//Waits for CS2 input for iteration test
	uint8_t word5[] = "test 5";
	lcd_write_word(word5);
	_delay_ms(4000);
	LCD_CLEAR;
	LCD_HOME;

}
void lcd_callback_lab2(void) {//ok
	if (counter > 9){
		counter = 0;	
	} else {
	switch(getButtonInput()){
		case 0b00100000:{// Knapp CS1
				//Skriver ut telleren paa posisjon 0
				LCD_CLEAR;
				LCD_HOME;
				lcd_write_int(counter);
				counter++;
				break;
			}
			case 0b01000000:{// Knapp CS2
				TestApp();
				break;
			}
			default:{
			
			break;
			}
		}
	}		
}
void lcd_blink_disable(void) {
	// Blinking "off"
	LCD.CTRLD &= ~LCD_BLINKEN_bm;
}

void lcd_set_blink(uint8_t rate) {//ok
	//Enables blinks and sett one of four blinkerates
	if (rate == 1) {	
		LCD.CTRLD = LCD_BLINKEN_bm | LCD_BLINKRATE_0Hz5_gc;
	} else if (rate == 2) {
		LCD.CTRLD = LCD_BLINKEN_bm | LCD_BLINKRATE_1Hz_gc;
	} else if (rate = 3) {
		LCD.CTRLD = LCD_BLINKEN_bm | LCD_BLINKRATE_2Hz_gc;
	} else if ( rate == 4) {
		LCD.CTRLD = LCD_BLINKEN_bm | LCD_BLINKRATE_4Hz_gc;
	}
}

void lcd_write_char_pos(const uint8_t data, uint8_t pos) {//ok
	//Iterates pos amount of place in lcd, and places the letter accordingly in the right place. 
	while(pos--){
		LCD.CTRLH = ' ';
	}
	LCD.CTRLH = data;
	/**/
}

void lcd_write_char(const uint8_t data) {//ok
	//Simply puts the char to in LCD.CTRLH register.  
	uint8_t width = 7;
	uint8_t x = 0;
	LCD.CTRLH = data;
}

void lcd_write_word(const uint8_t *data) {
	//Extra method to write the whole word on lcd. 
	uint8_t width = 7;
	uint8_t x = 0;
	
	while (width--)
	{
		if (data == '\0') {
				break; // Stop on NULL char
			}
		LCD.CTRLH = *data;
		data++;
	}
}

void lcd_write_int(int i) {//ok
	
	//Converts an int to a char, by adding a char '0' to the int, ultimatly converting it to ascii value;
	
	uint8_t x = i + '0';
	
	lcd_write_char(x);

}

uint8_t getButtonInput(void) {//ok
	
	//Gets input from the touch buttons, CS0, CS1, CS2 and CS3

	if (~PORTE_IN & 0x01) {
		return 0b00010000;
	} else if (~PORTE_IN & 0x02) {
	    return 0b00100000;
	} else if (~PORTE_IN & 0x04) {
		return 0b01000000;
	} else if (~PORTE_IN & 0x08) {
		return 0b10000000;
	} else 
		return 0b00000000;
}

void init_lcd(void) {//ok

	//Initiates ldc screen
	
	PR.PRGEN &= ~PR_LCD_bm;//Turns on pherical clock
	LCD.CTRLA = LCD_CLRDT_bm;//Clears LCD
	CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm;
	LCD.CTRLB = LCD_PRESC_bm | LCD_CLKDIV1_bm | LCD_CLKDIV0_bm | LCD_LPWAV_bm;//Sets frame rate to 64 Hz, 1/4 duty and 1/3 bias
	LCD.CTRLC = LCD_PMSK_gm; //Enables all segments on micro 
	LCD.CTRLA = LCD_ENABLE_bm | LCD_CLRDT_bm | LCD_SEGON_bm; //Enables LCD and all of its segments		
	LCD.CTRLF = 31;	//Set highest LCD contrast	
	LCD.CTRLG = LCD_TDG_14S_4C_gc | START_14SEG_4C; // Enables the 14-segment and sets start point to left of screen.	
	init_lcd_interrupts();
	
}
void init_lcd_interrupts(void) {//ok

	//Sets interrupt period to 32 frames
	LCD.INTCTRL = LCD_XIME4_bm | LCD_XIME3_bm | LCD_XIME2_bm | LCD_XIME1_bm | LCD_XIME0_bm | LCD_FCINTLVL0_bm | LCD_FCINTLVL1_bm;	
	//Enables all levels of interrupts, because we didn't knew which one will be triggered after CS were pressed. 
	PMIC_CTRL = PMIC_HILVLEN_bm  | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm ;	
	sei();
	
}
ISR (LCD_INT_vect) {
		
	//sets up ISR callback	
	cli();
	
	lcd_callback_lab2();
	sei();	
}

void init_buttons(void) {//ok
	//button initialization
	PORTE.DIR = 0b11110000;
	PORTE_PIN0CTRL = PORT_OPC_PULLUP_gc;
	PORTE_PIN1CTRL = PORT_OPC_PULLUP_gc;
	PORTE_PIN2CTRL = PORT_OPC_PULLUP_gc; 
	PORTE_PIN3CTRL = PORT_OPC_PULLUP_gc;
}
