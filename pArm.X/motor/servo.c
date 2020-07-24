#include "servo.h"
#include "../utils/defs.h"
#include "../utils/utils.h"


void writeAngle(const Servo *servo, uint16_t angle) {
    double time = mapf(angle * 1.0, .0, 180.0, .75, 2.25);
    writeMilliseconds(servo, time);
}

inline void writeMilliseconds(const Servo *servo, double ms) {
    *servo->dutyCycleRegister = (uint16_t) (FOSC / ((1 / ms) * 1000 * 64));
}

inline void writeValue(const Servo *servo, uint16_t dutyCycleValue) {
    *servo->dutyCycleRegister = dutyCycleValue;
}
