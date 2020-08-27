#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"

static inline void handleInterrupt(void) {
    // TODO
}

inline void move(motor_t *motor, uint16_t angle) {
    writeAngle(motor->servoHandler, angle);
    // TODO - setup expected ticks and actual ticks
}

inline void home(motor_t motors[MAX_MOTORS]) {
    foreach(motor_t, motor, motors) {
        // TODO - Define "home" position for each motor
        switch (motor->id) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
        }
    }
    // And setup interruptors for detecting the end
}

inline void freeze(motor_t *motor) {
    // TODO - Disable motor {id} interrupts / ticks so stop counting
    const volatile time_t ticks = motor->ticks;
    const double motorActualMillis = (double) (ticksToUs(ticks) * 1000);
    writeMilliseconds(motor->servoHandler, motorActualMillis);
}

inline double position_ms(motor_t *motor) {
    const time_t ticks = motor->ticks;
    return (double) (ticksToUs(ticks) * 1000);
}

inline double position(motor_t *motor) {
    const time_t ticks = motor->ticks;
    const double timeMillis = (double) (ticksToUs(ticks) * 1000);
    return mapf(timeMillis, MIN_PULSE_MS, MAX_PULSE_MS, .0, 180.);
}
