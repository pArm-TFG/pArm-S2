/*
 * Copyright (C) 2020 - present | pArm-S2 by Javinator9889
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *
 * Created by Javinator9889 on 2020 - pArm-S1.
 */

/* 
 * File: servo.h
 * Author: Javinator9889
 * Comments: Servo controller header file
 * Revision history: 1.0
 */

#ifndef SERVO_H
#define	SERVO_H

#include <stdint.h>
#include "../utils/defs.h"
#include "../utils/types.h"

#define usToTicks(_us)      ( (clockCyclesPerMicrosecond() * _us) / PRESCALE )
#define ticksToUs(_ticks)   ( ((unsigned)_ticks * PRESCALE) / clockCyclesPerMicrosecond() )
#define MIN_PULSE_WIDTH     usToTicks(((unsigned long) MIN_PULSE_MS * 1000))
#define MAX_PULSE_WIDTH     usToTicks(((unsigned long) MAX_PULSE_MS * 1000))

// Servo definition
typedef struct {
    volatile uint16_t *dutyCycleRegister;
    volatile uint_fast8_t *limit_switch_value;
    double64_t home;
    double64_t min_angle;
    double64_t max_angle;
} servo_t;

//extern volatile uint_fast8_t limit_switch_map[4];

void SERVO_write_angle(const servo_t *servo, double64_t angle_rad);
void SERVO_write_milliseconds(const servo_t *servo, double64_t ms);
void SERVO_write_value(const servo_t *servo, uint16_t dtc_value);
double64_t SERVO_from_angle_to_ms(double64_t angle_rad);

#endif	/* SERVO_H */

