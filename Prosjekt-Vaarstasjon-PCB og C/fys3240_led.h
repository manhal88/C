/*   
Function     : Code for a microcontrollerprogram flashing leds according to pushbuttons 
*/

#include <avr/io.h>
#include <inttypes.h>
#define F_CPU 32000000
#include <util/delay.h>


#define DEVICE ATXMEGA128B1
#define turnOnLed3() PORTB_OUT |= 0b10000000
#define turnOnLed2() PORTB_OUT |= 0b01000000
#define turnOnLed1() PORTB_OUT |= 0b00100000
#define turnOnLed0() PORTB_OUT |= 0b00010000


void init_32M_clock(void);
void put_on_leds(uint8_t value);
void flash_on_keypress(void);
void led_counter(void);
void init_leds(void);
void init_buttons_and_leds(void);

uint8_t getButtonInput(void);
void init_buttons(void);
