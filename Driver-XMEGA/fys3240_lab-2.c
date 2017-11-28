/* 
Function     : Precode for a microcontrollerprogram interfacing the ADC
*/



#include "fys3240_adcb.h"
#include <stdlib.h>
#include <avr/pgmspace.h>
/* ISR's here */

volatile uint8_t val;
volatile uint8_t counter;
volatile uint8_t flag;

ISR(ADCB_CH0_vect) {
		
		cli();
		adcb_callback();
		sei();
		
	
	// Interrupt flag is cleared upon return from ISR
}
ISR (LCD_INT_vect) {
		
	//sets up ISR callback	
		cli();
		getButtonInput();
		lcd_for_adc_callback();
		sei();	
		
}


int main(void)
{
	uint8_t pot_meter;
	init_lcd();
	init_buttons();
	ADCBInit();
	sei();
	
	
    while(1){
		
		LCD_CLEAR;
		LCD_HOME;
		ADCB.CH0.CTRL |= 0x80;
	   	_delay_ms(5);
		show_value(val);
		_delay_ms(25);
	}
}


 
void ADCBInit(void) 
{
	
	
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
	CLK.PSCTRL = CLK_PSADIV0_bm; 
	ADCB.PRESCALER = ADC_PRESCALER2_bm;		
	ADCB.REFCTRL = ADC_REFSEL0_bm;
	ADCB.EVCTRL = 0x00;
	ADCB.INTFLAGS = ADC_CH0IF_bm;
	ADCB.CH0.INTCTRL = ADC_CH_INTLVL0_bm | ADC_CH_INTLVL1_bm;
	ADCB.CTRLA |= ADC_ENABLE_bm;
	ADCB.CTRLB = ADC_CONMODE_bm;
	PORTB.DIR &= ~0x02;
	//ADCB.CH0.MUXCTRL = ADC_CH_MUXINT0_bm;
	//ADCB.CH0.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm;
	
	 
	

}

void adcb_callback(void)
{
		val = ADCB.CH0.RES;
		flag = 1;
	
	/* Code for the callback function for adc-interrupts here. */
}

void lcd_for_adc_callback(void)
{
	if(!(val & 0x800)) { 
	show_value(val);
	}	
}

static uint8_t conv_ascii(uint8_t val)
{
	
	val = val + '0';
	
	return val;
	/*uint8_t current_val = '0';
	uint8_t next_val = '0';
	uint8_t return_val[4] = "    ";
	
		current_val = val / 100;
		//lcd_write_char(return_val[0]);
		if (current_val == 0)
		{
			current_val = val/10;
			if (current_val == 0)
			{
				return_val[0] = current_val + 48;
				lcd_write_char('a');
				lcd_write_char(return_val[0]);
				return_val[1] = '\0';
			}else{
				return_val[0] = current_val + 48;
				return_val[1] = (val - (current_val*10)) + 48;
				return_val[2] = '\0';
				lcd_write_char('b');
				lcd_write_char(return_val[0]);
				lcd_write_char(return_val[1]);
			}
			
		}else{
			
			return_val[0] = current_val + 48;
			lcd_write_char('c');
			lcd_write_char(return_val[0]);
			next_val = val - (current_val*100);
			current_val = (val - (current_val*100))/10;			
			return_val[1] = current_val + 48;
			lcd_write_char(return_val[1]);
			return_val[2] = (next_val - (current_val*10)) + 48;
			lcd_write_char(return_val[2]);
			return_val[3] = '\0';
		}			
			
	return return_val ;*/
}

void show_value(uint8_t val){
	
	LCD_CLEAR;
	LCD_HOME;
	//lcd_write_word(conv_ascii(val));
	//conv_ascii(val);
	uint8_t tmp;
	uint8_t val1;
	uint8_t val2;
	uint8_t val3;
	
	tmp = val / 100;
	
	if (tmp) {
		val1 = conv_ascii(tmp);
		lcd_write_char(val1);
	
	} else{ 
	    val1 = 0;
	}	
	val = val - tmp*100;
	tmp = val / 10;
		
	if (tmp) {
		val2 = conv_ascii(tmp);
		lcd_write_char(val2);
	
	} else{ 
		lcd_write_char('0');
	}
		
	val= val - tmp*10;
	val3 = conv_ascii(val);
	lcd_write_char(val3);
			
	
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

void lcd_write_word(uint8_t *data) {
	//Extra method to write the whole word on lcd. 
	uint8_t width = 1;
	uint8_t x = 0;
	lcd_write_char(data[0]);	
	while (width--)
	{
		if (data == '\0') {
				break; // Stop on NULL char
			}
		lcd_write_char(*data);	
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
		return ADCB.CH0.MUXCTRL = ADC_CH_MUXINT0_bm;
	} else if (~PORTE_IN & 0x02) {
	    return ADCB.CH0.MUXCTRL = ADC_CH_MUXINT1_bm;
	} else if (~PORTE_IN & 0x04) {
		return ADCB.CH0.MUXCTRL = ADC_CH_MUXINT2_bm;
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


void init_buttons(void) {//ok
	//button initialization
	PORTE.DIR = 0b11110000;
	PORTE_PIN0CTRL = PORT_OPC_PULLUP_gc;
	PORTE_PIN1CTRL = PORT_OPC_PULLUP_gc;
	PORTE_PIN2CTRL = PORT_OPC_PULLUP_gc; 
	PORTE_PIN3CTRL = PORT_OPC_PULLUP_gc;
}
