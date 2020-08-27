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

servo_t base_servo = {&SDC1, .0, .0, .0};
servo_t lower_arm_servo = {&SDC2, .0, .0, .0};
servo_t upper_arm_servo = {&SDC3, .0, .0, .0};
servo_t end_effector_servo = {&PDC1, .0, .0, .0};

motor_t base_motor = {&base_servo, 0ULL, 0};
motor_t lower_arm_motor = {&lower_arm_servo, 0ULL, 1};
motor_t upper_arm_motor = {&upper_arm_servo, 0ULL, 2};
motor_t end_effetor_motor = {&end_effector_servo, 0ULL, 3};

motors_t motors = {&base_motor, &lower_arm_motor, &upper_arm_motor, &end_effetor_motor};

void go_home(void) {
    move(motors.base_motor, motors.base_motor->servoHandler->home);
    move(motors.lower_arm, motors.lower_arm->servoHandler->home);
    move(motors.upper_arm, motors.upper_arm->servoHandler->home);
    move(motors.end_effector_arm, motors.end_effector_arm->servoHandler->home);
}

void move_xyz(const point_t xyz) {
    angle_t *angle = (angle_t *) malloc(sizeof(angle_t));
    inverse_kinematics(xyz, angle);
    move_angle(*angle);
//    move(motors.base_motor, angle->theta0);
//    move(motors.lower_arm, angle->theta1);
//    move(motors.upper_arm, angle->theta2);
}

void move_angle(const angle_t angle) {
    move(motors.base_motor, angle.theta0);
    move(motors.lower_arm, angle.theta1);
    move(motors.upper_arm, angle.theta2);
}

void move_waiting(const angle_t angle) {
    
}

void stop_moving(void) {
    
}

point_t get_position(void) {
    
}

angle_t get_angles(void) {
    
}
