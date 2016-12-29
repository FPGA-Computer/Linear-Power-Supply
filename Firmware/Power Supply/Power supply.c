/*
 * Power_supply.c
 *
 * Created: 20/12/2016 2:30:31 PM
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

int16_t Setpoint_Voltage, AccumError ,Prev_Voltage;

void Voltage_PID(void)
{ 
  int16_t Actual_Voltage, Error;
  int32_t DAC_Adj;

  Actual_Voltage =  ADC_Conv(ADC_VOLT);
  Error = Setpoint_Voltage - Actual_Voltage;

  DAC_Adj = (int32_t) (PARM_P*Error+PARM_I*AccumError+PARM_D*(Actual_Voltage-Prev_Voltage));

  if(DAC_Adj > DAC_ADJ_LIMIT)
    DAC_Adj = DAC_ADJ_LIMIT;
  else if(DAC_Adj < -DAC_ADJ_LIMIT)
    DAC_Adj = -DAC_ADJ_LIMIT;

  if(DAC_Adj!=0)
    DAC_Set_Relative(DAC_Adj);
    
  Prev_Voltage = Actual_Voltage;
}

inline void UpdateDisplay(void)
{
  uint8_t i;
  uint16_t value;

  for(i=0;i<2;i++)
  {
    LCD_TextCursor(0,i);

    value = ADC_Conv(i);
    
    if(i)
    {
      if(value > CUR_OFFSET)
        value -= CUR_OFFSET;
      else
        value = 0;
        
      value /= CUR_SCALE;
    }
        
    LCD_PutUFixed(value);
    LCD_Putch(i?LCD_CH_A:LCD_CH_V); 
  }

}

int main(void)
{ 
  uint8_t update_count=0;
  
  Init_HW();
      
  while(1)
  {
    if(TIFR & (1<<TOV2))    // Poll for Timer2 overflow
    {
      TIFR = (1<<TOV2);     // Clear Timer2 overflow
      
      Voltage_PID();

      if(Encoder_Count)
      {
        cli();
        Setpoint_Voltage += Encoder_Count;
        Encoder_Count = 0;
        sei();

        if(Setpoint_Voltage < MIN_OUTPUT)
          Setpoint_Voltage = MIN_OUTPUT;
        else if (Setpoint_Voltage > MAX_OUTPUT)
          Setpoint_Voltage = MAX_OUTPUT;
      }

      #ifdef SETPOINT_ROW
      LCD_TextCursor(0,SETPOINT_ROW);
      LCD_PutUFixed(Setpoint_Voltage);
      LCD_Putch(LCD_CH_V);
      #endif
      
      if(update_count)
        update_count--;
      else
      {
        UpdateDisplay();
        update_count=UPDATE_COUNT;
      }
    }
  }
}
