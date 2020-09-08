#include <stdbool.h>
#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"

static inline void handleInterrupt(void) {
    // TODO
}

inline void MOTOR_move(motor_t *motor, uint16_t angle) {
    SERVO_write_angle(motor->servoHandler, angle);
    long double current_angle = motor->ticks;
    long double expected_time_us = MOTOR_elapsed_time_us(fabsl(angle - current_angle));
    //    long double total_time_secs = SECS_PER_DEGREE * angle;
    motor->movement_duration = expected_time_us;
    motor->movement_finished = false;
    // TODO - setup expected ticks and actual ticks
}

inline void MOTOR_home(motor_t *motor) {
    MOTOR_move(motor, motor->servoHandler->home);
    // And setup interruptors for detecting the end
}

inline void MOTOR_freeze(motor_t *motor) {
    // TODO - Disable motor {id} interrupts / ticks so stop counting
    const volatile time_t ticks = motor->ticks;
    const double motorActualMillis = (double) (ticksToUs(ticks) * 1000);
    SERVO_write_milliseconds(motor->servoHandler, motorActualMillis);
}

inline double MOTOR_position_ms(motor_t *motor) {
    const time_t ticks = motor->ticks;
    return (double) (ticksToUs(ticks) * 1000);
}

inline double MOTOR_position(motor_t *motor) {
    const time_t ticks = motor->ticks;
    const double timeMillis = (double) (ticksToUs(ticks) * 1000);
    return mapf(timeMillis, MIN_PULSE_MS, MAX_PULSE_MS, .0, 180.);
}
