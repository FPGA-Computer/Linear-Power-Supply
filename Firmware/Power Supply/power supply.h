/*
 * power_supply.h
 *
 * Created: 20/12/2016 2:45:18 PM
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

#ifndef POWER_SUPPLY_H_
#define POWER_SUPPLY_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay_basic.h>

#define SLEEP_MODE  0

#include "hardware.h"
#include "Nokia 5510.h"

#define DIV_FRACTION  100
#define DIV_DIGITS    10

#define SETPOINT_ROW  3

#define UPDATE_RATE   5
#define UPDATE_COUNT  (F_CPU/UPDATE_RATE/262144L)

// PID parameters
//#define Int_Error_Range 100

#define PARM_P          9
#define PARM_I          4
#define PARM_D          9
#define DAC_ADJ_LIMIT   32767

void Voltage_PID(void);
extern int16_t Setpoint_Voltage, AccumError ,Prev_Voltage;
#endif /* POWER SUPPLY_H_ */
