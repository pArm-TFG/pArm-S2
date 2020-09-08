/* 
 * File: planner.h 
 * Author: Javinator9889
 * Comments: The planner that controls how the arm moves
 * Revision history: v1.0
 */

#include "planner.h"

#include <xc.h>
#include "../motor/motor.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../motor/kinematics.h"
#include "../arm_config.h"

servo_t base_servo = {&SDC1, .0, LOWER_UPPER_MIN_ANGLE, LOWER_UPPER_MAX_ANGLE};
servo_t lower_arm_servo = {&SDC2, .0, LOWER_ARM_MIN_ANGLE, LOWER_ARM_MAX_ANGLE};
servo_t upper_arm_servo = {&SDC3, .0, UPPER_ARM_MIN_ANGLE, UPPER_ARM_MAX_ANGLE};
servo_t end_effector_servo = {&PDC1, .0, .0, 180.};

motor_t base_motor = {&base_servo, 0ULL, 0};
motor_t lower_arm_motor = {&lower_arm_servo, 0ULL, 1};
motor_t upper_arm_motor = {&upper_arm_servo, 0ULL, 2};
motor_t end_effetor_motor = {&end_effector_servo, 0ULL, 3};

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
}

void PLANNER_move_angle(const angle_t angle) {
    MOTOR_move(motors.base_motor, angle.theta0);
    MOTOR_move(motors.lower_arm, angle.theta1);
    MOTOR_move(motors.upper_arm, angle.theta2);
}

void PLANNER_move_waiting(const angle_t angle) {
    
}

void PLANNER_stop_moving(void) {
    
}

point_t PLANNER_get_position(void) {
    
}

angle_t PLANNER_get_angles(void) {
    
}
