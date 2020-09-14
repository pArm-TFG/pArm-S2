#include <stdbool.h>
#include <float.h>
#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"
#include "../utils/types.h"

static inline double64_t us_to_deg(motor_t *motor) {
    return (motor->angle_us / US_PER_DEGREE);
}

static inline double64_t us_to_rad(motor_t *motor) {
    return ((motor->angle_us * (MATH_PI / 180.0F)) / US_PER_DEGREE);
}

inline void MOTOR_move(motor_t *motor, double64_t angle) {
    double64_t current_angle = motor->angle_us;
    double64_t expected_time_us = MOTOR_elapsed_time_us(fabsl(angle - current_angle));
    motor->movement_duration = expected_time_us;
    motor->movement_finished = false;
    SERVO_write_angle(motor->servoHandler, angle);
    motor->TMR_Start();
}

inline double64_t MOTOR_home(motor_t motors[MAX_MOTORS]) {
    double64_t max_duration = LDBL_MIN;
    foreach(motor_t, motor, motors) {
        MOTOR_move(motor, motor->servoHandler->home);
        if (motor->movement_duration > max_duration)
            max_duration = motor->movement_duration;
    }
    return max_duration;
}

inline void MOTOR_freeze(motor_t *motor) {
    // Disable motor interrupts so stop counting
    motor->TMR_Stop();
    // Get current position and fix the angle to its value
    SERVO_write_angle(motor->servoHandler, MOTOR_position_deg(motor));
}

inline double64_t MOTOR_position_us(motor_t *motor) {
    return motor->angle_us;
}

inline double64_t MOTOR_position_rad(motor_t *motor) {
    return us_to_rad(motor);
}

inline double64_t MOTOR_position_deg(motor_t *motor) {
    return us_to_deg(motor);
}
