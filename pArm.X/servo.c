#include "servo.h"


void writeAngle(Servo *servo, uint16_t angle) {
//    printf("%d\n", angle);
//    double time = preciseMap(angle * 1.0, .0, 180.0, .75, 2.25);
    double time = mapf(angle * 1.0, .0, 180.0, .75, 2.25);
//    printf("time: %f | angle: %d\n", time, angle);
    writeMilliseconds(servo, time);
}

inline void writeMilliseconds(Servo *servo, double ms) {
    /*const uint16_t value = (uint16_t) (FOSC / ((1 / ms) * 1000 * 32));
    printf("\t%d\n", value);*/
    *servo->dutyCycleRegister = (uint16_t) (FOSC / ((1 / ms) * 1000 * 64));
}

inline void writeValue(Servo *servo, uint16_t dutyCycleValue) {
    *servo->dutyCycleRegister = dutyCycleValue;
}

/*void write(uint8_t number, uint8_t angle) {
    time_t time = map(angle, 0, 180, 2808, 8424);
    writeValue(number, time);
}

void writeValue(uint8_t number, time_t dutyCycleValue) {
//    uint16_t dutyCycleValue = map(value, 0, 1800, 0, 37400);
    switch (number) {
        case 1:
            SDC1 = dutyCycleValue;
            break;
        case 2:
            SDC2 = dutyCycleValue;
            break;
        case 3:
            SDC3 = dutyCycleValue;
            break;
        case 4:
            SDC4 = dutyCycleValue;
            break;
    }
}*/
