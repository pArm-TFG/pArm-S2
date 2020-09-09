/* 
 * File: planner.h 
 * Author: Javinator9889
 * Comments: The planner that controls how the arm moves
 * Revision history: v1.0
 */

#include "planner.h"

#include <xc.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "../motor/motor.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../motor/kinematics.h"
#include "../arm_config.h"
#include "../timers/tmr3.h"
#include "../timers/tmr4.h"
#include "../timers/tmr5.h"

servo_t base_servo = {&SDC1, .0, LOWER_UPPER_MIN_ANGLE, LOWER_UPPER_MAX_ANGLE};
servo_t lower_arm_servo = {&SDC2, .0, LOWER_ARM_MIN_ANGLE, LOWER_ARM_MAX_ANGLE};
servo_t upper_arm_servo = {&SDC3, .0, UPPER_ARM_MIN_ANGLE, UPPER_ARM_MAX_ANGLE};
servo_t end_effector_servo = {&PDC1, .0, .0, 180.};

motor_t base_motor = {&base_servo, 0ULL, .0F, .0F, false, TMR3_Start, TMR3_Stop};
motor_t lower_arm_motor = {&lower_arm_servo, 1ULL, .0F, .0F, false, TMR4_Start, TMR4_Stop};
motor_t upper_arm_motor = {&upper_arm_servo, 2ULL, .0F, .0F, false, TMR5_Start, TMR5_Stop};
motor_t end_effetor_motor = {&end_effector_servo, 3ULL, .0F, .0F, false, NULL, NULL};

motors_t motors = {&base_motor, &lower_arm_motor, &upper_arm_motor, &end_effetor_motor};

void PLANNER_go_home(void) {
    MOTOR_move(motors.base_motor, motors.base_motor->servoHandler->home);
    MOTOR_move(motors.lower_arm, motors.lower_arm->servoHandler->home);
    MOTOR_move(motors.upper_arm, motors.upper_arm->servoHandler->home);
    MOTOR_move(motors.end_effector_arm, motors.end_effector_arm->servoHandler->home);
}

void PLANNER_move_xyz(const point_t xyz) {
    angle_t *angle = (angle_t *) malloc(sizeof(angle_t));
    inverse_kinematics(xyz, angle);
    PLANNER_move_angle(*angle);
    free(angle);
}

void PLANNER_move_angle(const angle_t angle) {
    MOTOR_move(motors.base_motor, angle.theta0);
    MOTOR_move(motors.lower_arm, angle.theta1);
    MOTOR_move(motors.upper_arm, angle.theta2);
}

void PLANNER_move_waiting(const angle_t angle) {
    double64_t max_angle = LDBL_MIN;
    double64_t diff_base = fabsl(angle.theta0 - MOTOR_position_rad(motors.base_motor));
    double64_t diff_lower_arm = fabsl(angle.theta1 - MOTOR_position_rad(motors.lower_arm));
    double64_t diff_upper_arm = fabsl(angle.theta2 - MOTOR_position_rad(motors.upper_arm));
    max_angle = max(diff_base, max_angle);
    max_angle = max(diff_lower_arm, max_angle);
    max_angle = max(diff_upper_arm, max_angle);
    
    double64_t expected_time = MOTOR_elapsed_time_us(max_angle);
    PLANNER_move_angle(angle);
    delay_us(expected_time);
}

void PLANNER_stop_moving(void) {
    MOTOR_freeze(motors.base_motor);
    MOTOR_freeze(motors.lower_arm);
    MOTOR_freeze(motors.upper_arm);
}

point_t *PLANNER_get_position(void) {
    point_t *position = (point_t *) malloc(sizeof(point_t));
    angle_t angles = {
        MOTOR_position_rad(motors.base_motor),
        MOTOR_position_rad(motors.lower_arm),
        MOTOR_position_rad(motors.upper_arm)
    };
    forward_kinematics(angles, position);
    return position;
}

angle_t *PLANNER_get_angles(void) {
    angle_t *angles = (angle_t *) malloc(sizeof(angle_t));
    angles->theta0 = MOTOR_position_rad(motors.base_motor);
    angles->theta1 = MOTOR_position_rad(motors.lower_arm);
    angles->theta2 = MOTOR_position_rad(motors.upper_arm);
    
    return angles;
}
