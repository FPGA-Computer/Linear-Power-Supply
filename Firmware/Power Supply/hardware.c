/*
 * hardware.c
 *
 * Created: 22/12/2016 6:03:49 AM
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

// Rotary encoder - positive count is clockwise
volatile int8_t Encoder_Count;
static uint8_t Encoder_Prev;

ISR(ENC_CLK_IRQ)
{
  uint8_t Enc_Status;

  Enc_Status = REG(ENC_PORT,PIN) & (ENC_CLK|ENC_DT);

  if((Encoder_Prev == ENC_DT) && (Enc_Status == ENC_CLK))
    Encoder_Count--;    
  else if((!Encoder_Prev) && (Enc_Status == (ENC_CLK|ENC_DT)))
    Encoder_Count++;

  Encoder_Prev = Enc_Status;
}

uint16_t ADC_Conv(uint8_t ch)
{
  uint16_t Value;
  
  // ADC: internal 2.56V reference,Input channel
  ADMUX = ADC_REF|ch;

  // ADC: Enable, Start Conversion, single conversion, clock divider = 32 (250kHz)
  ADCSRA = (1<<ADEN)|ADC_CLKDIV|(1<<ADSC);

  while(!(ADCSRA & (1<<ADIF)))
    /* wait for ADC */;

  // Clear ADC Interrupt flag
  ADCSRA = (1<<ADEN)|ADC_CLKDIV|(1<<ADIF);
 
  Value = ADC + ADC_OFFSET;
   
  return(Value);
}

void DAC_Set_Relative(int16_t Value)
{
  
  if(Value<0)
  {
    Value = -Value;
    REG(DAC_PORT,PORT) |= DAC_0;       // '1' to decrease output
  }
  else
  REG(DAC_PORT,PORT) &= ~DAC_0;      // '0' to increase voltage
  
  REG(DAC_PORT,DDR) |= DAC_0;          // GPIO = output
  _delay_loop_2(Value);                // softwae loop Value*4 cycles
  REG(DAC_PORT,DDR) &= ~DAC_0;         // tristate pin
}

void Init_HW(void)
{
  // Disable pullup.
  SFIOR |= (1<<PUD);
  REG(DAC_PORT,PORT) |= DAC_0;
  REG(DAC_PORT,DDR) |= DAC_0;
  
  DDRB = 0xff;
  DDRD = ~(ENC_CLK|ENC_DT|ENC_SW);
  
 // _delay_loop_2(0xffff);     // *4 cycles
   
  LCD_Init();

  // Timer2: Fast PWM, non-inverting mode, prescaler = 1024 -> 30.5Hz interrupt
  TCCR2 = (1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<COM20)|(1<<CS22)|(1<<CS21)|(1<<CS20);

  // INT0 = level change
  MCUCR = (1<<ISC00);

  // Enable INT0, Global Interrupts
  GICR = (1<<INT0);
  sei();
  
  REG(ENABLE_PORT,PORT) = ENABLE;
  Setpoint_Voltage = MIN_OUTPUT;
}
