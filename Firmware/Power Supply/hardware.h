/*
 * hardware.h
 *
 * Created: 22/12/2016 6:03:25 AM
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


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define F_CPU           8000000UL

#define CONCAT(X,Y)     (X ## Y)
#define REG(X,Y)        CONCAT(Y,X)

#define LCD_PORT        B
#define LCD_DC          (1<<PORTB2)
#define LCD_MISO        (1<<PORTB4)

// internal reference
#define ADC_REF         ((1<<REFS1)|(1<<REFS0))

#define ADC_VOLT        0
#define ADC_CURRENT     1

//#define ADC_NOISE_REDUCTION

// ADC Clock = 8MHz/32 = 250kHz
#define ADC_CLKDIV  ((1<<ADPS2)/*|(1<<ADPS1)*/|(1<<ADPS0))

#define ENC_PORT        D
#define ENC_CLK         (1<<PORTD2)
#define ENC_DT          (1<<PORTD4)
#define ENC_SW          (1<<PORTD3)

#define ENC_CLK_IRQ     INT0_vect
#define ENC_SW_IRQ      INT1_vect

#define DAC_PORT        C
#define DAC_0           (1<<PORTD2)

#define ENABLE_PORT     B
#define ENABLE          (1<<PORTB1)

#define MAX_OUTPUT      1000
#define MIN_OUTPUT      10

#define CUR_SCALE       5

// Trim board dependent offsets
#define ADC_OFFSET      2
#define CUR_OFFSET      6

extern volatile int8_t Encoder_Count;

void Init_HW(void);
uint16_t ADC_Conv(uint8_t ch);
void DAC_Set_Relative(int16_t Value);

#endif /* HARDWARE_H_ */
