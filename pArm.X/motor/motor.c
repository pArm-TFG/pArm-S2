#include <stdbool.h>
#include <float.h>
#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../utils/time.h"

static inline double64_t us_to_deg(motor_t *motor) {
    return (motor->angle_us / US_PER_DEGREE);
}

static inline double64_t us_to_rad(motor_t *motor) {
    return ((motor->angle_us * (MATH_PI / 180.0F)) / US_PER_DEGREE);
}

inline void MOTOR_move(motor_t *motor, double64_t angle) {
    double64_t current_angle = us_to_rad(motor);
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

void MOTOR_calibrate(motor_t *motor) {
    // Init the angle to minimum long double value
    motor->angle_us = LDBL_MIN;
    // Move the motor to 0 radians
    SERVO_write_angle(motor->servoHandler, .0F);
    // and wait until the interruptor is pressed.
    // As maybe the interruptor can be not pressed, wait
    // a maximum amount of time equals to a 180 degrees spin
    // or the "movement_finished" flag to be true
    const time_t max_waiting_time =
            (time_t) (TIME_now_us() + (US_PER_DEGREE * 180.0F));
    while ((*motor->servoHandler->limit_switch_value != 1) ||
            (TIME_now_us() >= max_waiting_time) ||
            motor->movement_finished);
    *motor->servoHandler->limit_switch_value = 0;
    // Disable the PWM signal, so the motor stops moving
    SERVO_write_value(motor->servoHandler, 0U);
    // and move it to an arbitrary position at 30 degrees
    SERVO_write_angle(motor->servoHandler, 30.0F);
    // waiting until the movement should finish
    delay_us(30.0F * US_PER_DEGREE);
    // Finally, plan a movement again to 0 radians
    motor->movement_duration = 30.0F * US_PER_DEGREE;
    motor->movement_finished = false;
    SERVO_write_angle(motor->servoHandler, .0F);
    motor->TMR_Start();
    // This time, wait until the interruptor is pressed
    // or the movement flag finished is set to true
    while ((*motor->servoHandler->limit_switch_value != 1) ||
            motor->movement_finished);
    // When done, finish the counter and update the minimum angle
    // with the difference in us obtained
    motor->TMR_Stop();
    *motor->servoHandler->limit_switch_value = 0;
    motor->movement_finished = true;
    // If the motor is correctly calibrated, this difference may be zero
    // or approximately zero. In other case, the difference will be
    // the new minimum angle the motor can reach
    double64_t min_angle_us = fabsl(motor->angle_us - motor->movement_duration);
    motor->servoHandler->min_angle = (min_angle_us / US_PER_DEGREE);
}
