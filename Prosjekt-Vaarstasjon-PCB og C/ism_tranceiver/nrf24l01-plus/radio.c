/* Copyright (c) 2007 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision: 2310 $
 */ 

/* MODIFIED BY ELAB */
#include "../ism_tranceiver.h"

#include "hal_nrf.h"
#include "radio.h"

/* Radio interrupt indication */
static volatile bool active_radio_interrupt = false;

/** The payload sent over the radio. Also contains the recieved data. 
 * Should be read with radio_get_pload_byte(). */
static uint8_t pload[RF_PAYLOAD_LENGTH];

/** The current status of the radio. Should be set with radio_set_status(), 
 * and read with radio_get_status().
 */
static radio_status_t status;

ISR (NRF24L01_IRQ_VECTOR) {
	
	puts("radio irq");	
	if (READ(NRF24L01_IRQ) == 0)
	active_radio_interrupt = true;
}


void radio_send_packet(uint8_t *packet, uint8_t length)
{
	hal_nrf_write_tx_pload(packet, length);      // load message into radio
  
	/* send packet */
	HIGH(NRF24L01_CHIP_ENABLE);
	_delay_us(50);
	LOW(NRF24L01_CHIP_ENABLE);

	radio_set_status (RF_BUSY);                 // trans. in progress; RF_BUSY
}

radio_status_t radio_get_status (void)
{
  return status;
}

uint8_t radio_get_pload_byte (uint8_t byte_index)
{
  return pload[byte_index];
}

void radio_set_status (radio_status_t new_status)
{
  status = new_status;
}

void radio_irq(void) {
	
	/* Check if an interrupt is triggered */
	if (READ(NRF24L01_IRQ) == 0) { //(active_radio_interrupt == true) {
		
		switch(hal_nrf_get_clear_irq_flags ()) {
			
			/* If max retries is reached. Flush TX FIFO, avoid FIFO jam */
			case (1<<HAL_NRF_MAX_RT):
				hal_nrf_flush_tx();
				radio_set_status (RF_MAX_RT);
				break;
      
			/* Packet sent */
			case (1<<HAL_NRF_TX_DS):
				radio_set_status (RF_TX_DS);
				break;
      
			/* Packet received */
			case (1<<HAL_NRF_RX_DR):                  
				while (!hal_nrf_rx_fifo_empty ()) {
					hal_nrf_read_rx_pload(pload);
				}
				radio_set_status (RF_RX_DR);
				break;
  
			/* Acknowledge payload received */
			case ((1<<HAL_NRF_RX_DR)|(1<<HAL_NRF_TX_DS)):
				while (!hal_nrf_rx_fifo_empty ()) {
					hal_nrf_read_rx_pload(pload);
				}
				radio_set_status (RF_TX_AP);
				break;
  
			default:
				break;    
		}
	
		active_radio_interrupt = false;
	}
}
