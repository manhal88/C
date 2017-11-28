/**
 * \file
 *
 * \brief ST7565R display controller driver.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 *    AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/* MODIFIED BY ELAB ! */

#include "../ism_tranceiver.h"
#include "../avr/spi.h"

#include "st7565r.h"

void st7565r_write_command(uint8_t command) {

		spi_mode3();
        LOW(LCD_CHIP_SELECT);
        LOW(LCD_A0);
		spi_transmit(&command);
        HIGH(LCD_CHIP_SELECT);
}

void st7565r_write_data(uint8_t data) {
	
		spi_mode3();
        LOW(LCD_CHIP_SELECT);
        HIGH(LCD_A0);
		spi_transmit(&data);
        LOW(LCD_A0);
        HIGH(LCD_CHIP_SELECT);
}

/**
 * \brief Perform a soft reset of the LCD controller
 *
 * This functions will reset the LCD controller by sending the reset command.
 * \note this funtions should not be confused with the \ref st7565r_hard_reset()
 * function, this command will not control the RST pin.
 */
void st7565r_soft_reset(void) {
	
        st7565r_write_command(ST7565R_CMD_RESET);
}

/**
 * \brief Perform a hard reset of the LCD controller
 *
 * This functions will reset the LCD controller by setting the reset pin low.
 * \note this funtions should not be confused with the \ref st7565r_soft_reset()
 * function, this command will control the RST pin.
 */
void st7565r_hard_reset(void) {
	
        LOW(LCD_RESET);
        _delay_ms(100);
        HIGH(LCD_RESET);
        _delay_ms(100);
}

/**
 * \brief Set current page in display RAM
 *
 * This command is usually followed by the configuration of the column address
 * because this scheme will provide access to all locations in the display
 * RAM.
 *
 * \param address the page address
 */
void st7565r_set_page_address(uint8_t address)
{
        // Make sure that the address is 4 bits (only 8 pages)
        address &= 0x0F;
        st7565r_write_command(ST7565R_CMD_PAGE_ADDRESS_SET(address));
}

/**
 * \brief Set current column in display RAM
 *
 * \param address the column address
 */
void st7565r_set_column_address(uint8_t address)
{
        // Make sure the address is 7 bits
        address &= 0x7F;
        st7565r_write_command(ST7565R_CMD_COLUMN_ADDRESS_SET_MSB(address >> 4));
        st7565r_write_command(ST7565R_CMD_COLUMN_ADDRESS_SET_LSB(address & 0x0F));
}

/**
 * \brief Set the display start draw line address
 *
 * This function will set which line should be the start draw line for the LCD.
 */
void st7565r_set_display_start_line_address(uint8_t address)
{
        // Make sure address is 6 bits
        address &= 0x3F;
        st7565r_write_command(ST7565R_CMD_START_LINE_SET(address));
}

/**
 * \brief Turn the LCD display on
 *
 * This function will turn on the LCD.
 */
void st7565r_display_on(void)
{
        st7565r_write_command(ST7565R_CMD_DISPLAY_ON);
}

/**
 * \brief Turn the LCD display off
 *
 * This function will turn off the LCD.
 */
void st7565r_display_off(void)
{
        st7565r_write_command(ST7565R_CMD_DISPLAY_OFF);
}

/**
 * \brief Sets all LCD pixels on
 *
 * This function can be used to test the LCD by setting all pixels on, this will
 * not affect the current LCD RAM.
 *
 * \param pixels_on if true all the LCD pixels are turned on, false the display
 *                  is back in normal mode displaying what is in the display
 *                  RAM.
 */
void st7565r_set_all_pixels(uint8_t pixels_on)
{
        if (pixels_on) {
                st7565r_write_command(ST7565R_CMD_DISPLAY_ALL_POINTS_ON);
        } else {
                st7565r_write_command(ST7565R_CMD_DISPLAY_ALL_POINTS_OFF);
        }
}

#define ST7565R_DISPLAY_CONTRAST_MIN	30
#define ST7565R_DISPLAY_CONTRAST_MAX	40
/**
 * \brief Set the LCD contrast level
 *
 * \warning This will set the voltage for the LCD, settings this value too high
 * may result in damage to the LCD. Hence the limit for these settings must be
 * defined in the \ref conf_st7565r.h file.
 *
 * Contrast values outside the max and min values will be clipped to the defined
 * \ref ST7565R_DISPLAY_CONTRAST_MAX and \ref ST7565R_DISPLAY_CONTRAST_MIN.
 *
 * \param contrast a number between 0 and 63 where the max values is given by
 *                 the LCD.
 *
 * \retval contrast the contrast value written to the LCD controller
 */
uint8_t st7565r_set_contrast(uint8_t contrast)
{
        if (contrast < ST7565R_DISPLAY_CONTRAST_MIN) {
                contrast = ST7565R_DISPLAY_CONTRAST_MIN;
        }
        if (contrast > ST7565R_DISPLAY_CONTRAST_MAX) {
                contrast = ST7565R_DISPLAY_CONTRAST_MAX;
        }
        st7565r_write_command(ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET);
        st7565r_write_command(ST7565R_CMD_ELECTRONIC_VOLUME(contrast));
        return contrast;
}

/**
 * \brief Invert all pixels on the device
 *
 * This function will invert all pixels on the LCD
 *
 */
void st7565r_display_invert_enable(void)
{
        st7565r_write_command(ST7565R_CMD_DISPLAY_REVERSE);
}

/**
 * \brief Disable invert of all pixels on the device
 *
 * This function will disable invert on all pixels on the LCD
 *
 */
void st7565r_display_invert_disable(void)
{
        st7565r_write_command(ST7565R_CMD_DISPLAY_NORMAL);
}

void st7565r_init(void)
{
        // Do a hard reset of the LCD display controller
        st7565r_hard_reset();

        // Set the A0 pin to the default state (command)
        LOW(LCD_A0);

        // The column address is set to increasing
        st7565r_write_command(ST7565R_CMD_ADC_NORMAL);

        // Non-inverted display
        st7565r_display_invert_disable();

        // The common mode scan direction is reversed COM31->COM0
        st7565r_write_command(ST7565R_CMD_REVERSE_SCAN_DIRECTION);

        // Set the voltage bias ratio to 1/6
        st7565r_write_command(ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33);

        // Set booster circuit, voltage regulator and voltage follower all to on
        st7565r_write_command(ST7565R_CMD_POWER_CTRL_ALL_ON);

        // Set the booster ratio to 2X,3X,4X
        st7565r_write_command(ST7565R_CMD_BOOSTER_RATIO_SET);
        st7565r_write_command(ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X);

        // Set voltage resistor ratio to 1
        st7565r_write_command(ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1);

        /* Set contrast to min value, no need to check return value as the contrast
        is set to the defined min*/
        st7565r_set_contrast(ST7565R_DISPLAY_CONTRAST_MIN);

        // Turn on the display
        st7565r_display_on();
}