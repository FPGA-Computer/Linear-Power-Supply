/*
 * Nokia 5510.c
 *
 * Created: 22/12/2016 5:55:30 AM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 

#include "power supply.h"

const __flash uint8_t  LCD_InitCmd[] =
{
  LCD_CMD_FH1, LCD_Contrast|LCD_ContrastDefault, LCD_TempCtrl, LCD_SetBias, LCD_CMD_FH0, LCD_Disp_Off
};

// non-ASCII to save space

const __flash uint8_t LCD_CharROM[] =
{
  0x3E,0x51,0x49,0x45,0x3E,		//  '0'
  0x00,0x42,0x7F,0x40,0x00,		//  '1'
  0x62,0x51,0x51,0x49,0x46,		//  '2'
  0x22,0x41,0x49,0x49,0x36,		//  '3'
  0x18,0x14,0x12,0x7F,0x10,		//  '4'
  0x47,0x45,0x45,0x45,0x39,		//  '5'
  0x3C,0x4A,0x49,0x49,0x30,		//  '6'
  0x03,0x01,0x79,0x05,0x03,		//  '7'
  0x36,0x49,0x49,0x49,0x36,		//  '8'
  0x06,0x49,0x49,0x29,0x1E,		//  '9'
  0x00,0x60,0x60,0x00,0x00,		//  '.'
  0x7C,0x12,0x11,0x12,0x7C,		//  'A'
  0x0F,0x30,0x40,0x30,0x0F,		//  'V'
  0x00,0x00,0x00,0x00,0x00,   // Space
};

void SPI_Write(uint8_t byte)
{
  SPDR=byte;

  while(!(SPSR & (1<<SPIF)))
    ;
}

void SPI_FWrite(const __flash uint8_t *ptr, uint8_t size)
{
  while (size--)
  { SPDR=*ptr++;

    while(!(SPSR & (1<<SPIF)))
      ;
  }
}

inline void LCD_Cls(void)
{ uint16_t i;

  LCD_Home();
  LCD_DataMode();

  for(i=LCD_MAX_X*LCD_MAX_Y;i;i--)
  SPI_Write(0);
}

void LCD_Init(void)
{
  SPCR = 0;					                      // SPI: Off, MISO = 0
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);		// SPI: Mode 0, Master, 8MHz/2, MISO = Input
                                          // MISO -> input with pullup, letting LCD out of reset.
  LCD_CmdMode();
  SPI_FWrite(LCD_InitCmd,sizeof(LCD_InitCmd));
  LCD_Cls();

  LCD_CmdMode();
  SPI_Write(LCD_Disp_On);
}

// graphic cursor
void LCD_Moveto(uint8_t Col, uint8_t Row)
{
  LCD_CmdMode();
  SPI_Write(LCD_Addr_X | Col);
  SPI_Write(LCD_Addr_Y | Row);
}

void LCD_Putch(uint8_t ch)
{ 
  LCD_DataMode();
  SPI_FWrite(LCD_CharROM+(ch*(LCD_SYM_WIDTH-1)),LCD_SYM_WIDTH-1);
  SPI_Write(0);
}

void LCD_PutUInt(uint16_t Num, uint8_t Format)
{
  uint8_t Div = DIV_DIGITS;

  do
  {
    if((Format!=LeadingSpace)||(Num>=Div)||(Div==1))
    {
      Format = LeadingZero;
      LCD_Putch(LCD_CH_0+Num/Div);
      Num %= Div;
    }
    else
      LCD_Putch(LCD_CH_SPACE);

    Div /= 10;
  } while(Div);
}

void LCD_PutUFixed(uint16_t value)
{
  LCD_PutUInt(value/DIV_FRACTION,LeadingSpace);
  LCD_Putch(LCD_Ch_Dot);
  LCD_PutUInt(value%DIV_FRACTION,LeadingZero);
}
