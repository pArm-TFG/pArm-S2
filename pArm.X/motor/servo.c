#include "servo.h"
#include "../utils/defs.h"
#include "../utils/utils.h"


void SERVO_write_angle(const servo_t *servo, double64_t angle) {
    double time = SERVO_from_angle_to_ms(angle);
    SERVO_write_milliseconds(servo, time);
}

inline void SERVO_write_milliseconds(const servo_t *servo, double64_t ms) {
    *servo->dutyCycleRegister = (uint16_t) (FOSC / ((1 / ms) * 1000 * 64));
}

inline void SERVO_write_value(const servo_t *servo, uint16_t dutyCycleValue) {
    *servo->dutyCycleRegister = dutyCycleValue;
}

inline double64_t SERVO_from_angle_to_ms(double64_t angle) {
    return mapf(angle * 1.0, .0, 180.0, .75, 2.25);
}
