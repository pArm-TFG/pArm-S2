/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
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
#define MOTOR_elapsed_time_us(radians)  ((US_PER_DEGREE * radians) / (MATH_PI / 180.F))

typedef struct {
    servo_t *servoHandler;
    const uint8_t id;
    volatile double64_t movement_duration;
    volatile double64_t angle_us;
    volatile bool movement_finished;
    TMR_func TMR_Start;
    TMR_func TMR_Stop;
} motor_t;


static void handleInterrupt(void);
void MOTOR_move(motor_t *motor, double64_t angle);
void MOTOR_home(motor_t motor[MAX_MOTORS]);
void MOTOR_freeze(motor_t *motor);
double64_t MOTOR_position_us(motor_t *motor);
double64_t MOTOR_position_rad(motor_t *motor);
double64_t MOTOR_position_deg(motor_t *motor);

#endif	/* MOTOR_H */

