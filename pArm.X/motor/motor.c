#include <stdbool.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include "motor.h"
#include "../printf/io.h"
#include "../utils/utils.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../utils/time.h"

static inline double64_t us_to_deg(double64_t us) {
    return (us / US_PER_DEGREE);
}

static inline double64_t us_to_rad(double64_t us) {
    return ((us * (MATH_PI / 180.0F)) / US_PER_DEGREE);
}

static inline double64_t rad_to_us(double64_t rad) {
    return (rad * MATH_TRANS * US_PER_DEGREE);
}

static inline double64_t deg_to_us(double64_t deg) {
    return (deg * US_PER_DEGREE);
}

inline void MOTOR_move(motor_t *motor, double64_t angle_rad) {
    double64_t current_angle = us_to_rad(motor->angle_us);
    double64_t expected_time_us = MOTOR_elapsed_time_us(fabsl(angle_rad - current_angle));
    motor->clockwise = (angle_rad > current_angle)
            ? 1
            : -1;
    motor->movement_duration = expected_time_us;
    motor->movement_finished = false;
    SERVO_write_angle(motor->servoHandler, angle_rad);
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
    SERVO_write_milliseconds(motor->servoHandler, (motor->angle_us * 1000.0F));
}

inline double64_t MOTOR_position_us(motor_t *motor) {
    return motor->angle_us;
}

inline double64_t MOTOR_position_rad(motor_t *motor) {
    return us_to_rad(motor->angle_us);
}

inline double64_t MOTOR_position_deg(motor_t *motor) {
    return us_to_deg(motor->angle_us);
}

inline bool check_motor_finished(motor_t *motor, time_t max_waiting_time) {
    return (*motor->servoHandler->limit_switch_value == 1) ? true : 
        (TIME_now_us() >= max_waiting_time) ? true :
            motor->movement_finished;
}

char MOTOR_calibrate(motor_t *motor) {
#ifdef DEBUG_ENABLED    
    printf("[SETUP]\tCalibrating motor %d\n", motor->id);
#endif
    // Init the angle to minimum long double value
    motor->angle_us = LDBL_MIN;
    // Move the motor to 0 radians
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tMoving motor %d to position 0\n", motor->id);
#endif
    SERVO_write_angle(motor->servoHandler, .0F);
    // and wait until the interruptor is pressed.
    // As maybe the interruptor can be not pressed, wait
    // a maximum amount of time equals to a 180 degrees spin
    // or the "movement_finished" flag to be true
    const time_t max_waiting_time =
            (time_t) (TIME_now_us() + (US_PER_DEGREE * 180.0F));
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tWaiting at most %LG s\n", (max_waiting_time / 1E6));
#endif
    while (!check_motor_finished(motor, max_waiting_time));
    const bool timeout_happened = ((TIME_now_us() >= max_waiting_time) == 1);
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tTimeout? %d\n", timeout_happened);
#endif
    *motor->servoHandler->limit_switch_value = 0;
    // Disable the PWM signal, so the motor stops moving
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tDisabling PWM signal\n");
#endif
    SERVO_write_value(motor->servoHandler, 0U);
    // Timeout happened, so return error
    if (timeout_happened)
        return EXIT_FAILURE;
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tMoving to 30 degrees\n");
#endif
    // and move it to an arbitrary position at 30 degrees
    SERVO_write_angle(motor->servoHandler, (MATH_PI / 6));
    double64_t duration_us = rad_to_us(MATH_PI / 6);
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tExpected duration: %LG us\n", duration_us);
#endif
    // waiting until the movement should finish
    delay_us(rad_to_us(duration_us));
    // Finally, plan a movement again to 0 radians
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tFinishing calibration... Moving to 0 again\n");
#endif
    motor->movement_duration = duration_us;
    motor->movement_finished = false;
    SERVO_write_angle(motor->servoHandler, .0F);
    motor->TMR_Start();
    // This time, wait until the interruptor is pressed
    // or the movement flag finished is set to true
    while ((*motor->servoHandler->limit_switch_value != 1) ||
            motor->movement_finished);
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tMovement for motor %d finished!\n", motor->id);
#endif
    // When done, finish the counter and update the minimum angle
    // with the difference in us obtained
    motor->TMR_Stop();
    *motor->servoHandler->limit_switch_value = 0;
    motor->movement_finished = true;
    // If the motor is correctly calibrated, this difference may be zero
    // or approximately zero. In other case, the difference will be
    // the new minimum angle the motor can reach
    double64_t min_angle_us = fabsl(motor->angle_us - motor->movement_duration);
    motor->servoHandler->min_angle = us_to_rad(min_angle_us);
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tMinimum angle for motor %d is: %LG rad\n", motor->id, motor->servoHandler->min_angle);
#endif
    // Return OK
    return EXIT_SUCCESS;
}
