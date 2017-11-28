/* Partially based on original code for the KS0108 by Stephane Rey */

/**************************************************************************/
/*! 
    @file     smallfonts.c
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

/* MODIFIED BY ELAB ! */

#include "../ism_tranceiver.h"
#include "font.h"


/* 5x8 font */
const PROGMEM uint8_t font_5x8_table[] = {
	
    0x00,0x00,0x00,0x00,0x00, /* Space */
    0x00,0x00,0x4f,0x00,0x00, /* ! */
    0x00,0x07,0x00,0x07,0x00, /* " */
    0x14,0x7f,0x14,0x7f,0x14, /* # */
    0x24,0x2a,0x7f,0x2a,0x12, /* 0x */
    0x23,0x13,0x08,0x64,0x62, /* % */
    0x36,0x49,0x55,0x22,0x20, /* & */
    0x00,0x05,0x03,0x00,0x00, /* ' */
    0x00,0x1c,0x22,0x41,0x00, /* ( */
    0x00,0x41,0x22,0x1c,0x00, /* ) */
    0x14,0x08,0x3e,0x08,0x14, /* // */
    0x08,0x08,0x3e,0x08,0x08, /* + */
    0x50,0x30,0x00,0x00,0x00, /* , */
    0x08,0x08,0x08,0x08,0x08, /* - */ 
    0x00,0x60,0x60,0x00,0x00, /* . */
    0x20,0x10,0x08,0x04,0x02, /* / */
    0x3e,0x51,0x49,0x45,0x3e, /* 0 */
    0x00,0x42,0x7f,0x40,0x00, /* 1 */
    0x42,0x61,0x51,0x49,0x46, /* 2 */
    0x21,0x41,0x45,0x4b,0x31, /* 3 */
    0x18,0x14,0x12,0x7f,0x10, /* 4 */
    0x27,0x45,0x45,0x45,0x39, /* 5 */
    0x3c,0x4a,0x49,0x49,0x30, /* 6 */
    0x01,0x71,0x09,0x05,0x03, /* 7 */
    0x36,0x49,0x49,0x49,0x36, /* 8 */
    0x06,0x49,0x49,0x29,0x1e, /* 9 */
    0x00,0x36,0x36,0x00,0x00, /* : */
    0x00,0x56,0x36,0x00,0x00, /* ; */
    0x08,0x14,0x22,0x41,0x00, /* < */
    0x14,0x14,0x14,0x14,0x14, /* = */
    0x00,0x41,0x22,0x14,0x08, /* > */
    0x02,0x01,0x51,0x09,0x06, /* ? */
    0x3e,0x41,0x5d,0x55,0x1e, /* @ */
    0x7e,0x11,0x11,0x11,0x7e, /* A */
    0x7f,0x49,0x49,0x49,0x36, /* B */
    0x3e,0x41,0x41,0x41,0x22, /* C */
    0x7f,0x41,0x41,0x22,0x1c, /* D */
    0x7f,0x49,0x49,0x49,0x41, /* E */
    0x7f,0x09,0x09,0x09,0x01, /* F */
    0x3e,0x41,0x49,0x49,0x7a, /* G */
    0x7f,0x08,0x08,0x08,0x7f, /* H */
    0x00,0x41,0x7f,0x41,0x00, /* I */
    0x20,0x40,0x41,0x3f,0x01, /* J */
    0x7f,0x08,0x14,0x22,0x41, /* K */
    0x7f,0x40,0x40,0x40,0x40, /* L */
    0x7f,0x02,0x0c,0x02,0x7f, /* M */
    0x7f,0x04,0x08,0x10,0x7f, /* N */
    0x3e,0x41,0x41,0x41,0x3e, /* O */
    0x7f,0x09,0x09,0x09,0x06, /* P */
    0x3e,0x41,0x51,0x21,0x5e, /* Q */
    0x7f,0x09,0x19,0x29,0x46, /* R */
    0x26,0x49,0x49,0x49,0x32, /* S */
    0x01,0x01,0x7f,0x01,0x01, /* T */
    0x3f,0x40,0x40,0x40,0x3f, /* U */
    0x1f,0x20,0x40,0x20,0x1f, /* V */
    0x3f,0x40,0x38,0x40,0x3f, /* W */
    0x63,0x14,0x08,0x14,0x63, /* X */
    0x07,0x08,0x70,0x08,0x07, /* Y */
    0x61,0x51,0x49,0x45,0x43, /* Z */
    0x00,0x7f,0x41,0x41,0x00, /* [ */
    0x02,0x04,0x08,0x10,0x20, /* \ */ 
    0x00,0x41,0x41,0x7f,0x00, /* ] */
    0x04,0x02,0x01,0x02,0x04, /* ^ */
    0x40,0x40,0x40,0x40,0x40, /* _ */
    0x00,0x00,0x03,0x05,0x00, /* ` */
    0x20,0x54,0x54,0x54,0x78, /* a */
    0x7F,0x44,0x44,0x44,0x38, /* b */
    0x38,0x44,0x44,0x44,0x44, /* c */
    0x38,0x44,0x44,0x44,0x7f, /* d */
    0x38,0x54,0x54,0x54,0x18, /* e */
    0x04,0x04,0x7e,0x05,0x05, /* f */
    0x08,0x54,0x54,0x54,0x3c, /* g */
    0x7f,0x08,0x04,0x04,0x78, /* h */
    0x00,0x44,0x7d,0x40,0x00, /* i */
    0x20,0x40,0x44,0x3d,0x00, /* j */
    0x7f,0x10,0x28,0x44,0x00, /* k */
    0x00,0x41,0x7f,0x40,0x00, /* l */
    0x7c,0x04,0x7c,0x04,0x78, /* m */
    0x7c,0x08,0x04,0x04,0x78, /* n */
    0x38,0x44,0x44,0x44,0x38, /* o */
    0x7c,0x14,0x14,0x14,0x08, /* p */
    0x08,0x14,0x14,0x14,0x7c, /* q */
    0x7c,0x08,0x04,0x04,0x00, /* r */
    0x48,0x54,0x54,0x54,0x24, /* s */
    0x04,0x04,0x3f,0x44,0x44, /* t */
    0x3c,0x40,0x40,0x20,0x7c, /* u */
    0x1c,0x20,0x40,0x20,0x1c, /* v */
    0x3c,0x40,0x30,0x40,0x3c, /* w */
    0x44,0x28,0x10,0x28,0x44, /* x */
    0x0c,0x50,0x50,0x50,0x3c, /* y */
    0x44,0x64,0x54,0x4c,0x44, /* z */
};
