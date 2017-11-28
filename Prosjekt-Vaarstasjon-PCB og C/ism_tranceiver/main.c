/*
 * Copyright (c) 2015 - 2016 Elektronikklaboratoriet
 *                           Fysisk Institutt
 *                           Universitetet i Oslo
 *
 *							 All rights reserved
 */

#include "ism_tranceiver.h"

#include "ndh-c12832a1z/graphic_lcd.h"
#include "ndh-c12832a1z/st7565r.h"

#include "hdc1050/hdc1050.h"

#include "nrf24l01-plus/hal_nrf.h"
#include "nrf24l01-plus/radio.h"
#include "nrf24l01-plus/radio_pl.h"

#include "real_time_clock.h"

static void radio_receiver_mode(void);
static void radio_transmitter_mode(void);

/* Radio payload */
uint8_t pload_pl[RF_PAYLOAD_LENGTH];

static void error(void) {
	while(true) {
		TOGGLE(LED_ORANGE);
		TOGGLE(LED_BLUE);
		_delay_ms(50);
	}
}

static void enable_graphic_display(void) {
	
	OUTPUT(LCD_CHIP_SELECT);
	OUTPUT(LCD_RESET);
	OUTPUT(LCD_A0);
	OUTPUT(LCD_BACKLIGHT);
	st7565r_init();
	graphic_lcd_clear_screen();
	graphic_lcd_write(1, 0, "hello ism transceiver!");
	LCD_BACKLIGHT_ON();
}

static void display_local_temperature_and_humidity(void) {

	uint8_t data[3];
	char display_buffer[30]; 

	if (hdc1050_read_temperature(&data[0], &data[1]))
		puts("failed reading temperature");
	if (hdc1050_read_humidity(&data[2]))
		puts("failed reading humidity");	
	sprintf(display_buffer, "local:  %d,%dC %d%%", data[0], data[1], data[2]);
	graphic_lcd_clear_screen();														 
	graphic_lcd_write(0, 0, display_buffer);
}


int main (void) {
	   
	uint8_t radio_address[] = { 0xeb, 0x01, 0xeb, 0x02, 0xeb }; 
	   
	/* Toggle LED if hardware initialization failed */
	if (init_hardware())
		error();
			
	/* Enable interrupts */
	sei();	

	LED_ORANGE_ON();
	
	/* Radio test if swithc is pressed at power up */
	if (READ(DOOR_BELL_SWITCH) == 0) {
		
		_delay_ms(100);
		
		if (READ(DOOR_BELL_SWITCH) == 0) {		
			
			#ifdef CONCENTRATOR_BOARD
				enable_graphic_display();
				graphic_lcd_write(0, 0, "Test mode:");
				graphic_lcd_write(1, 0, "Constant carrier");
				graphic_lcd_write(2, 0, "F0 = 2402MHz");
				graphic_lcd_write(3, 0, "RF power = 0dBm");
			#endif
			
			LED_BLUE_ON();			
			LED_ORANGE_ON();			
			
			hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
			_delay_ms(10);
			
			hal_nrf_set_operation_mode(HAL_NRF_PTX);
			hal_nrf_set_pll_mode(HAL_NRF_PLL_LOCK);
			
			#define CONTINOUS_WAVE 7
			hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) | (1<<CONTINOUS_WAVE)));
			
			HIGH(NRF24L01_CHIP_ENABLE);
			
			for(;;);
		}
	}
	
	#ifdef CONCENTRATOR_BOARD
	
		puts("concentrator board");
		enable_graphic_display();
		display_local_temperature_and_humidity();
		
		/* Enable radio in RX mode with payload in acknowledge */
		radio_pl_init (radio_address, HAL_NRF_PRX);
		radio_receiver_mode();
		
	#else
	
		puts("sensor board");
		
		/* Enable radio in TX mode with payload in acknowledge */
		radio_pl_init (radio_address, HAL_NRF_PTX);
		radio_transmitter_mode();		
			
	#endif

	return EXIT_SUCCESS;
}

			
static void radio_transmitter_mode(void) {

	while(true)	{
		
		/* Wait until the packet is sent */
		do {
			radio_irq ();
		} while ((radio_get_status ()) == RF_BUSY);

		/* Set blue LED if data is sent, or acknowledge payload is received */
		if ((radio_get_status ()) == RF_TX_DS || (radio_get_status ()) == RF_TX_AP)
			LED_BLUE_ON();

		if (radio_get_status () == RF_TX_AP) {
			
			/* If payload is one, DOOR_BELL_SWITCH on receiver side was pushed */
			if (radio_get_pload_byte(0))
				LED_ORANGE_ON();
		}

		_delay_ms(100);
		
		LED_BLUE_OFF();
		LED_ORANGE_OFF();
		
		_delay_ms(100);
		
		/* Setup and put the payload on the FIFO */
		pload_pl[0] = 0;

		/* If door bell switch is pressed */
		if (READ(DOOR_BELL_SWITCH) == 0)
			pload_pl[0] = 1;
			
		/* Add temperature and humidity to payload */
		if (hdc1050_read_temperature(&pload_pl[1], &pload_pl[2]))
			pload_pl[1] = 0xff;
			
		if (hdc1050_read_humidity(&pload_pl[3]))
			pload_pl[3] = 0xff;

		/* Send the packet */
		radio_send_packet(pload_pl, RF_PAYLOAD_LENGTH);
		
		puts("tx");
	}
}

static void radio_receiver_mode(void) {

	int radio_timeout_count_down = 0;
	uint8_t data[3];
	char display_buffer[30];
	bool update_display = false;
		
	/* Enable receiver */
	HIGH(NRF24L01_CHIP_ENABLE);

	while(true)	{
		
		/* Setup and put the ACK payload on the FIFO */
		pload_pl[0] = 0;

		/* If door bell switch is pressed */
		if (READ(DOOR_BELL_SWITCH) == 0)
			pload_pl[0] = 1;

		hal_nrf_write_ack_pload (0, pload_pl, RF_PAYLOAD_LENGTH);
		
		/* Run until either 110ms has lapsed
		 * OR there is data on the radio
		 */
		radio_timeout_count_down = 11;
		do {
			_delay_ms(10);
			radio_irq ();
				
		} while ((radio_get_status () == RF_IDLE) && radio_timeout_count_down--);

		/* Set blue LED if data is sent, or acknowledge payload is received */
		if ((radio_get_status ()) == RF_TX_DS || (radio_get_status ()) == RF_TX_AP)
			LED_BLUE_ON();

		if ((radio_get_status ()) == RF_RX_DR || (radio_get_status ()) == RF_TX_AP)	{
			
			/* If payload is one, DOOR_BELL_SWITCH on transmitter side was pushed */
			if (radio_get_pload_byte(0))
				LED_ORANGE_ON();
				
			if (update_display) {
				update_display = false;	
				sprintf(display_buffer, "sensor: %d,%dC %d%%", radio_get_pload_byte(1), radio_get_pload_byte(2), radio_get_pload_byte(3));
				graphic_lcd_write(2, 0, display_buffer);
			
				if (radio_get_pload_byte(0))
					graphic_lcd_write(3, 0, "hello!");
				
				printf("data %d %d %d %d\n", radio_get_pload_byte(0), radio_get_pload_byte(1), radio_get_pload_byte(2), radio_get_pload_byte(3));
			}
		}

		/* Set radio status to idle */
		radio_set_status (RF_IDLE);
		
		_delay_ms(500);
		LED_BLUE_OFF();
		LED_ORANGE_OFF();
		
		update_display = true;
		display_local_temperature_and_humidity();
		//if (second_tick) {
			//second_tick = false;
			//update_display = true;		
			//display_local_temperature_and_humidity();
		//}
		
	}
}

