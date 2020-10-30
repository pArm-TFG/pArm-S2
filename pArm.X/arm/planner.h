/*
 * 2020 | pArm-S2 by Javinator9889
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
    /**
     * pArm base motor, which moves along Y axis.
     */
    motor_t *base_motor;
    
    /**
     * pArm lower arm motor, which moves along X axis.
     */
    motor_t *lower_arm;
    
    /**
     * pArm upper motor, which moves along Z axis.
     */
    motor_t *upper_arm;
    
    /**
     * pArm end-effector mottor.
     */
    motor_t *end_effector_arm;
} 
/**
 * Simple struct containing pointers to every pArm motor.
 */
motors_t;

extern motors_t motors;

#ifdef LIMIT_SWITCH_ENABLED
/**
 * Initialize the planner so movements can be done.
 * 
 * @param barrier - the planner synchronization barrier.
 * @param switch_map - an array shared with {@link interrupts.h#_CNInterrupt}.
 */
void PLANNER_init(barrier_t *barrier, uint_fast8_t switch_map[4]);
#else
/**
 * Initialize the planner so movements can be done.
 * 
 * @param barrier - the planner synchronization barrier.
 */
void PLANNER_init(volatile barrier_t *barrier);
#endif

/**
 * Moves every motor to its home position.
 * 
 * @return the time, in us, that will pass until the movement is done.
 */
double64_t PLANNER_go_home(void);

/**
 * Moves to the specified position at point (x, y, z).
 * 
 * @param xyz - the final position.
 * @return the time, in us, that will pass until the movement is done.
 */
double64_t PLANNER_move_xyz(point_t xyz);

/**
 * Moves each motor to the specified angle (t0, t1, t2).
 * 
 * @param angle - the motors' angles.
 * @return the time, in us, that will pass until the movement is done.
 */
double64_t PLANNER_move_angle(angle_t angle);

/**
 * Moves the specified angle and waits until the movement is completed.
 * 
 * @param angle - the motors' angles.
 */
void PLANNER_move_waiting(angle_t angle);

/**
 * Immediately stops any possible movement of the motors.
 * 
 * @return EXIT_SUCCESS if there was any movement or EXIT_FAILURE if none.
 */
uint8_t PLANNER_stop_moving(void);

/**
 * Obtains the current end-effector position, in terms of (x, y, z).
 * 
 * @return a pointer to the position.
 */
point_t *PLANNER_get_position(void);

/**
 * Obtains the current motor angles, in terms of (t0, t1, t2).
 * 
 * @return a pointer to the angles.
 */
angle_t *PLANNER_get_angles(void);

#endif	/* PLANNER_H */

