#include <stdbool.h>
#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"
#include "../utils/types.h"

static inline void handleInterrupt(void) {
    // TODO
}

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

inline void MOTOR_home(motor_t *motor) {
    MOTOR_move(motor, motor->servoHandler->home);
    // And setup interruptors for detecting the end
}

inline void MOTOR_freeze(motor_t *motor) {
    // TODO - Disable motor {id} interrupts / ticks so stop counting
    const volatile time_t ticks = motor->angle_us;
    const double motorActualMillis = (double) (ticksToUs(ticks) * 1000);
    SERVO_write_milliseconds(motor->servoHandler, motorActualMillis);
}

inline double MOTOR_position_us(motor_t *motor) {
    return motor->angle_us;
}

inline double MOTOR_position_rad(motor_t *motor) {
    return us_to_rad(motor);
}

inline double MOTOR_position_deg(motor_t *motor) {
    return us_to_deg(motor);
}
