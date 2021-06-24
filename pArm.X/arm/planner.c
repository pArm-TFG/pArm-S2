/*
 * 2020 - present | pArm-S2 by Javinator9889
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

#include <xc.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "planner.h"
#include "../motor/motor.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../motor/kinematics.h"
#include "../arm_config.h"
#ifdef USE_MOTOR_TMRS
#include "../timers/tmr3.h"
#include "../timers/tmr4.h"
#include "../timers/tmr5.h"
#endif
#include "../utils/defs.h"
#include "../sync/barrier.h"
#ifdef DEBUG_ENABLED
#include "../printf/io.h"
#endif

servo_t base_servo = {&SDC1, NULL, (MATH_PI / 2), LOWER_UPPER_MIN_ANGLE, LOWER_UPPER_MAX_ANGLE};
servo_t lower_arm_servo = {&SDC2, NULL, MATH_PI, LOWER_ARM_MIN_ANGLE, LOWER_ARM_MAX_ANGLE};
servo_t upper_arm_servo = {&SDC3, NULL, DEG_55, UPPER_ARM_MIN_ANGLE, UPPER_ARM_MAX_ANGLE};
servo_t end_effector_servo = {&SDC4, NULL, .0, .0, 180.};

motor_t base_motor = {
    &base_servo,
    0ULL,
    .0F,
    .0F,
    0U,
    false,
    false,
#ifdef USE_MOTOR_TMRS
    TMR3_Start,
    TMR3_Stop
#else
    NULL,
    NULL
#endif
};

motor_t lower_arm_motor = {
    &lower_arm_servo,
    1ULL,
    .0F,
    .0F,
    0U,
    false,
    false,
#ifdef USE_MOTOR_TMRS
    TMR4_Start,
    TMR4_Stop
#else
    NULL,
    NULL
#endif
};

motor_t upper_arm_motor = {
    &upper_arm_servo,
    2ULL,
    .0F,
    .0F,
    0U,
    false,
    false,
#ifdef USE_MOTOR_TMRS
    TMR5_Start,
    TMR5_Stop
#else
    NULL,
    NULL
#endif
};

motor_t end_effetor_motor = {
    &end_effector_servo,
    3ULL,
    .0F,
    .0F,
    0U,
    false,
    false,
    NULL,
    NULL
};

motors_t motors = {&base_motor, &lower_arm_motor, &upper_arm_motor, &end_effetor_motor};
volatile barrier_t *PLANNER_barrier;

static inline double64_t expected_duration(angle_t angle) {
    double64_t max_angle = LDBL_MIN;
    double64_t diff_base = fabsl(angle.theta0 - MOTOR_position_rad(motors.base_motor));
    double64_t diff_lower_arm = fabsl(angle.theta1 - MOTOR_position_rad(motors.lower_arm));
    double64_t diff_upper_arm = fabsl(angle.theta2 - MOTOR_position_rad(motors.upper_arm));
    max_angle = max(diff_base, max_angle);
    max_angle = max(diff_lower_arm, max_angle);
    max_angle = max(diff_upper_arm, max_angle);

    return MOTOR_elapsed_time_us(max_angle);
}

static inline double64_t PLANNER_movement_expected_duration(void) {
    double64_t max_time = max(motors.base_motor->movement_duration, 
                              motors.lower_arm->movement_duration);
    return max(max_time, motors.upper_arm->movement_duration);
}

static inline void map_angle(angle_t *angle) {
    double64_t motor_t2 = (DEG_190 - (DEG_135 - angle->theta1) - angle->theta2);
    angle->theta0 = mapf(angle->theta0, .0F, MATH_PI, MATH_PI, .0F);
    angle->theta1 = mapf(angle->theta1, .0F, DEG_135, MATH_PI, DEG_45);
    angle->theta2 = motor_t2;
}

static inline void unmap_angle(angle_t *angle) {
    angle->theta0 = mapf(angle->theta0, MATH_PI, .0F, .0F, MATH_PI);
    angle->theta1 = mapf(angle->theta1, MATH_PI, DEG_45, .0F, DEG_135);
    angle->theta2 = (-angle->theta2 + DEG_190 - (DEG_135 - angle->theta1));
}

#ifdef LIMIT_SWITCH_ENABLED

void PLANNER_init(volatile barrier_t *barrier, uint_fast8_t switch_map[4]) {
    base_servo.limit_switch_value = &switch_map[0];
    lower_arm_servo.limit_switch_value = &switch_map[1];
    upper_arm_servo.limit_switch_value = &switch_map[2];
    end_effector_servo.limit_switch_value = &switch_map[3];
#else

void PLANNER_init(volatile barrier_t *barrier) {
#endif
    PLANNER_barrier = barrier;
#ifdef USE_MOTOR_TMRS
    TMR3_Initialize(motors.base_motor, PLANNER_barrier);
    TMR4_Initialize(motors.lower_arm, PLANNER_barrier);
    TMR5_Initialize(motors.upper_arm, PLANNER_barrier);
#endif
}

double64_t PLANNER_go_home(void) {
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMoving motors to home...\n");
#endif
    BARRIER_clr(PLANNER_barrier);
    MOTOR_move(motors.base_motor, motors.base_motor->servoHandler->home);
    MOTOR_move(motors.lower_arm, motors.lower_arm->servoHandler->home);
    MOTOR_move(motors.upper_arm, motors.upper_arm->servoHandler->home);
    return PLANNER_movement_expected_duration();
}

double64_t PLANNER_move_xyz(point_t xyz) {
    BARRIER_clr(PLANNER_barrier);
    angle_t *angle = (angle_t *) malloc(sizeof (angle_t));
    char ret = inverse_kinematics(xyz, angle);
    if (ret != EXIT_SUCCESS)
        return -1.0F;
    map_angle(angle);
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tMoving:\n"
            "\tbase motor to %Lfdeg\n"
            "\tlower motor to: %Lfdeg\n"
            "\tupper motor to: %Lfdeg\n", 
            (angle->theta0 * MATH_TRANS),
            (angle->theta1 * MATH_TRANS),
            (angle->theta2 * MATH_TRANS));
#endif
    MOTOR_move(motors.base_motor, angle->theta0);
    MOTOR_move(motors.lower_arm, angle->theta1);
    MOTOR_move(motors.upper_arm, angle->theta2);
    free(angle);
    return PLANNER_movement_expected_duration();
}

double64_t PLANNER_move_angle(angle_t angle) {
    BARRIER_clr(PLANNER_barrier);
    point_t *position = (point_t *) malloc(sizeof (point_t));
    char ret = forward_kinematics(angle, position);
    if (ret != EXIT_SUCCESS)
        return -1.0F;
    map_angle(&angle);
    MOTOR_move(motors.base_motor, angle.theta0);
    MOTOR_move(motors.lower_arm, angle.theta1);
    MOTOR_move(motors.upper_arm, angle.theta2);
    free(position);
    return PLANNER_movement_expected_duration();
}

void PLANNER_move_waiting(angle_t angle) {
    double64_t expected_time = PLANNER_move_angle(angle);
    delay_us(expected_time);
}

uint8_t PLANNER_stop_moving(void) {
    if (PLANNER_barrier->flag)
        return EXIT_FAILURE;
    MOTOR_freeze(motors.base_motor);
    MOTOR_freeze(motors.lower_arm);
    MOTOR_freeze(motors.upper_arm);
    BARRIER_set_done(PLANNER_barrier);
    return EXIT_SUCCESS;
}

point_t *PLANNER_get_position(void) {
    point_t *position = (point_t *) malloc(sizeof (point_t));
    angle_t *angles = PLANNER_get_angles();
    forward_kinematics(*angles, position);
    free(angles);
    return position;
}

angle_t *PLANNER_get_angles(void) {
    angle_t *angles = (angle_t *) malloc(sizeof (angle_t));
    angles->theta0 = MOTOR_position_rad(motors.base_motor);
    angles->theta1 = MOTOR_position_rad(motors.lower_arm);
    angles->theta2 = MOTOR_position_rad(motors.upper_arm);

    return angles;
}
