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
 * File: planner.h 
 * Author: Javinator9889
 * Comments: The planner that controls how the arm moves
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PLANNER_H
#define	PLANNER_H

#include <stdint.h>
#include "../motor/motor.h"
#include "../utils/types.h"
#include "../sync/barrier.h"

typedef struct {
    motor_t *base_motor;
    motor_t *lower_arm;
    motor_t *upper_arm;
    motor_t *end_effector_arm;
} motors_t;

extern motors_t motors;

#ifdef LIMIT_SWITCH_ENABLED
void PLANNER_init(barrier_t *barrier, uint_fast8_t switch_map[4]);
#else
void PLANNER_init(volatile barrier_t *barrier);
#endif
double64_t PLANNER_go_home(void);
double64_t PLANNER_move_xyz(point_t xyz);
double64_t PLANNER_move_angle(angle_t angle);
void PLANNER_move_waiting(angle_t angle);
uint8_t PLANNER_stop_moving(void);
point_t *PLANNER_get_position(void);
angle_t *PLANNER_get_angles(void);

#endif	/* PLANNER_H */

