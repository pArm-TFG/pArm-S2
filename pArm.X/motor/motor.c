#include "motor.h"
#include "../utils/utils.h"
#include "../utils/defs.h"

static inline void handleInterrupt(void) {
    // TODO
}

inline void move(Motor *motor, uint16_t angle) {
    writeAngle(motor->servoHandler, angle);
    // TODO - setup expected ticks and actual ticks
}

inline void home(Motor motors[MAX_MOTORS]) {
    foreach(Motor, motor, motors) {
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

inline void freeze(Motor *motor) {
    // TODO - Disable motor {id} interrupts / ticks so stop counting
    const volatile time_t ticks = motor->ticks;
    const double motorActualMillis = (double) (ticksToUs(ticks) * 1000);
    writeMilliseconds(motor->servoHandler, motorActualMillis);
}

inline double positionMs(Motor *motor) {
    const time_t ticks = motor->ticks;
    return (double) (ticksToUs(ticks) * 1000);
}

inline double position(Motor *motor) {
    const time_t ticks = motor->ticks;
    const double timeMillis = (double) (ticksToUs(ticks) * 1000);
    return mapf(timeMillis, MIN_PULSE_MS, MAX_PULSE_MS, .0, 180.);
}
