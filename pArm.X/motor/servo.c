/*
 * 2020 | pArm-S2 by Javinator9889
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *
 * Created by Javinator9889 on 2020 - pArm-S1.
 */

#include "servo.h"
#include "../utils/defs.h"
#include "../utils/utils.h"

//volatile uint_fast8_t limit_switch_map[4] = {0U};

void SERVO_write_angle(const servo_t *servo, double64_t angle_rad) {
    double64_t time = SERVO_from_angle_to_ms(angle_rad);
    SERVO_write_milliseconds(servo, time);
}

inline void SERVO_write_milliseconds(const servo_t *servo, double64_t ms) {
    SERVO_write_value(servo, (uint16_t) (1872 * ms));
}

inline void SERVO_write_value(const servo_t *servo, uint16_t dutyCycleValue) {
    *servo->dutyCycleRegister = dutyCycleValue;
}

inline double64_t SERVO_from_angle_to_ms(double64_t angle_rad) {
    return (double64_t) (((3.0 * angle_rad) / (2.0 * MATH_PI)) + 0.75);
}

inline uint16_t SERVO_from_angle_to_dc(double64_t angle_rad) {
    double64_t ms = SERVO_from_angle_to_ms(angle_rad);
    return (uint16_t) (1872 * ms);
}

inline double64_t SERVO_to_rad(const servo_t *servo) {
    return (double64_t) ((MATH_PI * (*servo->dutyCycleRegister - 1404)) / 2808);
}

inline uint16_t SERVO_position(const servo_t *servo) {
    return *servo->dutyCycleRegister;
}
