/*
 * Nokia 5510.h
 *
 * Created: 22/12/2016 5:57:03 AM
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


#ifndef LCD_Nokia5510_H_
#define LCD_Nokia5510_H_

#define LCD_MAX_X		84
#define LCD_MAX_Y		6
#define LCD_TEXT_WIDTH		5
#define LCD_SYM_WIDTH		  (LCD_TEXT_WIDTH+1)
#define LCD_MAXCOL		    (LCD_MAX_X/LCD_TEXT_WIDTH)
#define LCD_MAXROW		    LCD_MAX_Y

#define LCD_Home()		    LCD_Moveto(0,0)
#define LCD_CmdMode()		  { REG(LCD_PORT,PORT) &= ~LCD_DC;}
#define LCD_DataMode()		{ REG(LCD_PORT,PORT) |= LCD_DC;}

// Nokia 5110 commands
enum LCD_CMDS
{
  LCD_CMD_NOP = 0x00,
  LCD_CMD_FH0 = 0x20,
  LCD_CMD_FH1 = 0x21,
  // LCD_CMD_FH0
  LCD_Disp_Off = 0x08,
  LCD_Disp_On = 0x0c,
  LCD_Addr_Y = 0x40,
  LCD_Addr_X = 0x80,
  // LCD_CMD_FH1
  LCD_TempCtrl = 0x04,
  LCD_SetBias = 0x13,
  LCD_Contrast = 0x80,
  LCD_ContrastDefault = 0x40
};
enum LCD_CharSet
{
  LCD_CH_0,LCD_CH_1,LCD_CH_2,LCD_CH_3,LCD_CH_4,LCD_CH_5,LCD_CH_6,LCD_CH_7,LCD_CH_8,LCD_CH_9,LCD_Ch_Dot,LCD_CH_A,LCD_CH_V,LCD_CH_SPACE
};

enum LCD_Format
{ LeadingSpace, LeadingZero };

#define LCD_TextCursor(X,Y) LCD_Moveto(X*LCD_SYM_WIDTH,Y)

void LCD_Init(void);
void SPI_FWrite(const __flash uint8_t *ptr, uint8_t size);
void SPI_Write(uint8_t byte);
void LCD_Moveto(uint8_t Col, uint8_t Row);
void LCD_Putch(uint8_t ch);
void LCD_PutUFixed(uint16_t value);
#endif /* LCD_H_ */