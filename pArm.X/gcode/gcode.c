/*
 * File:   gcode.c
 * Author: javinator9889
 *
 * Created on 24 de agosto, 2020
 */

#include <float.h>
#include <stdlib.h>
#include <string.h>
#include "gcode.h"
#include "../arm/planner.h"
#include "../utils/utils.h"
#include "../utils/types.h"

char GCODE_BUFFER[MAX_BUFFER_LENGTH] = {0};
uint16_t cLength = 0;

inline angle_t GCODE_get_position(void) {

    return (angle_t) {
        motors.base_motor->angle_us,
                motors.lower_arm->angle_us,
                motors.upper_arm->angle_us
    };
}

float GCODE_parse_number(char code, float val) {
    char *ptr = GCODE_BUFFER;
    while ((long) ptr > 1 && (*ptr) && (long) ptr < (long) GCODE_BUFFER + cLength) {
        if (*ptr == code) return atof(ptr + 1);
        ptr = strchr(ptr, ' ') + 1;
    }
    return val;
}

GCODE_ret_t GCODE_process_command(const char* command) {
    GCODE_ret_t ret; // = {false, -1, NULL};

    foreach(char, code, command) {
        if (cLength < (MAX_BUFFER_LENGTH - 1))
            GCODE_BUFFER[cLength++] = code;
        if (code == '\n' || code == '\r') {
            GCODE_BUFFER[cLength] = '\0';
            break;
        }
    }
    int_fast16_t cmd = (int_fast16_t) GCODE_parse_number('G', -1.0F);
    switch (cmd) {
            // G0 X1.234 Y1.234 Z1.234
            // receives a position by the given
            // coordinates in XYZ.
            // If coordinates are missing,
            // throw an error
        case 0:
        {
            point_t position = {
                GCODE_parse_number('X', LDBL_MIN),
                GCODE_parse_number('Y', LDBL_MIN),
                GCODE_parse_number('Z', LDBL_MIN)
            };
            if (position.x == LDBL_MIN ||
                    position.y == LDBL_MIN ||
                    position.z == LDBL_MIN) {
                const char *msg = "Some coordinate is missing for GCODE G0\n";
                ret = (GCODE_ret_t){
                    true, // is_err
                    cmd,
                    msg
                };
            } else {
                ret = (GCODE_ret_t){
                    false, // is_err
                    cmd, // code
                    &position // the return value itself
                };
            }
            break;
        }
            // G1 X1.234 Y1.234 Z1.234
            // receives a set of angles by the given
            // values in XYZ.
            // Some angles can be missing, so keep the 
            // current angle in the other motors.
            // If three angles are missing, throw
            // an error
        case 1:
        {
            angle_t angles = {
                GCODE_parse_number('X', LDBL_MIN),
                GCODE_parse_number('Y', LDBL_MIN),
                GCODE_parse_number('Z', LDBL_MIN)
            };
            if (angles.theta0 == LDBL_MIN &&
                    angles.theta1 == LDBL_MIN &&
                    angles.theta2 == LDBL_MIN) {
                const char *msg = "Angles are missing for GCODE G1\n";
                ret = (GCODE_ret_t){
                    true, // is_err
                    cmd,
                    msg
                };
            } else {
                ret = (GCODE_ret_t){
                    false, // is_err
                    cmd, // code
                    &angles // the return value itself
                };
            }
            break;
        }
            // GCODE G28 - move to home
            // Leave this to main controller as it
            // knows the home of each motor and
            // sets the timer interrupt for them
        case 28:
        {
            ret = (GCODE_ret_t){
                false, // is_err
                cmd, // code
                NULL // the return value itself
            };
            break;
        }
            // The input GCODE is not recognized
            // (not implemented)
        default:
        {
            const char *msg = "Unknown GCODE G%d\n";
            ret = (GCODE_ret_t){
                true, // is_err
                cmd, // the code itself
                msg // the error message
            };
            break;
        }
    }

    // GCODE found so quit and return value
    if (cmd != -1)
        return ret;

    cmd = (int_fast16_t) GCODE_parse_number('M', -1.0F);
    switch (cmd) {
            // GCODE M1 - unconditional stop
            // When this command is received, the arm
            // must stop moving and keep current position
        case 1:
        {
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 10, // code
                NULL // the return value itself
            };
            break;
        }
            // GCODE M114 - get current position in XYZ
            // By default, obtains the position in angular
            // coordinates and leaves the conversion to 
            // main orchestrator
        case 114:
            // GCODE M280 - get current position in angles
        case 280:
        {
            angle_t current_position = GCODE_get_position();
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 10, // code
                &current_position // the return value itself
            };
            break;
        }
            // The input GCODE is not recognized
            // (not implemented)
        default:
        {
            const char *msg = "Unknown GCODE M%d\n";
            ret = (GCODE_ret_t){
                true, // is_err
                cmd * 10, // the code
                msg // the error message
            };
            break;
        }
    }

    // GCODE found so quit and return value
    if (cmd != -1)
        return ret;

    cmd = (int_fast16_t) GCODE_parse_number('I', -1.0F);
    switch (cmd) {
            // GCODE I1 - custom command for sending RSA public key
            // When received, the main orchestrator must send
            // both modulus (n) and public exponent (e)
            // so the other system can decrypt our messages.
        case 1:
            // GCODE I6 - generate new RSA keys
        case 6:
        {
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 100, // the code
                NULL // the return value itself
            };
            break;
        }
            // GCODE I5 - received the unsigned message
            // when verifying
        case 5:
            // GCODE I7 - received heartbeat so we know
            // the trusted device is still alive
        case 7:
        {
            char *msg = (char *) malloc(sizeof (char) * (cLength - 3));
            strncpy(msg, command + 3, (cLength - 3));
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 100, // the code
                strtoll(msg) // the msg
            };
            break;
        }
            // The input GCODE is not recognized
            // (not implemented)
        default:
        {
            const char *msg = "Unknown GCODE I%d\n";
            ret = (GCODE_ret_t){
                true, // is_err
                cmd * 100, // the code
                msg // the error message
            };
            break;
        }
    }
    return ret;
}
