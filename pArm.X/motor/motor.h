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
 * File: motor.h
 * Author: Javinator9889
 * Comments: The motor handler header file definition
 * Revision history: 1.0
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "servo.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../utils/defs.h"

#define MAX_MOTORS                      4U
#define US_PER_DEGREE                   5245.275704F
#define MOTOR_elapsed_time_us(rad)      (US_PER_DEGREE * rad * MATH_TRANS)

typedef struct {
    /**
     * Pointer storing the driver that manages the servomotor.
     */
    servo_t *servoHandler;
    
    /**
     * Motor unique identifier.
     */
    const uint8_t id;
    
    /**
     * The current movement duration, in us.
     */
    volatile double64_t movement_duration;
    
    /**
     * The current motor angle in us.
     */
    volatile double64_t angle_us;
    
    /**
     * Flag indicating that the motor has finished its movement.
     */
    volatile bool movement_finished;
    
    /**
     * Flag indicating whether the movement is clockwise or anticlockwise.
     */
    int8_t clockwise;
    
    /**
     * Volatile counter indicating the elapsed time since the movement started.
     */
    volatile time_t current_movement_count;
    
    /**
     * Function that initializes the timer attached to the motor.
     */
    TMR_func TMR_Start;
    
    /**
     * Function that finishes the timer attached to the motor.
     */
    TMR_func TMR_Stop;
} motor_t;

/**
 * Moves the motor to the specified angle in radians.
 * 
 * @param motor a pointer to the motor to move.
 * @param angle_rad the radians to move.
 */
void MOTOR_move(motor_t *motor, double64_t angle_rad);

/**
 * Freezes the motor at the current position.
 * 
 * @param motor a pointer to the motor to freeze.
 */
void MOTOR_freeze(motor_t *motor);

/**
 * Performs the motor calibration.
 * 
 * @param motor a pointer to the motor to calibrate.
 * @return EXIT_SUCCESS if calibration is OK or EXIT_FAILURE in other case.
 */
char MOTOR_calibrate(motor_t *motor);

/**
 * Gets the motor position as us.
 * 
 * @param motor a pointer to the motor.
 * @return the position in microseconds.
 */
double64_t MOTOR_position_us(motor_t *motor);

/**
 * Gets the motor actual position in radians.
 * 
 * @param motor a pointer to the motor.
 * @return the position in radians.
 */
double64_t MOTOR_position_rad(motor_t *motor);

/**
 * Gets the motor actual position in degrees.
 * 
 * @param motor a pointer to the motor.
 * @return the position in degrees.
 */
double64_t MOTOR_position_deg(motor_t *motor);

#endif	/* MOTOR_H */

