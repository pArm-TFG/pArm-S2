/*
 * File:   gcode.c
 * Author: javinator9889
 *
 * Created on 24 de agosto, 2020
 */

#include "gcode.h"
#include "../arm/planner.h"
#include "../utils/utils.h"
#include "../utils/types.h"

char GCODE_BUFFER[MAX_BUFFER_LENGTH] = {0};
uint16_t cLength = 0;

angle_t GCODE_get_position(void) {
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
        if (code == "\n" || code == "\r") {
            GCODE_BUFFER[cLength] = 0;
            break;
        }
    }
    int_fast16_t cmd = (int_fast16_t) GCODE_parse_number('G', -1.0F);
    switch (cmd) {
        case 0:
        {
            point_t position = {
                GCODE_parse_number('X', -1.0F),
                GCODE_parse_number('Y', -1.0F),
                GCODE_parse_number('Z', -1.0F)
            };
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd, // code
                &position // the return value itself
            };
            break;
        }
        case 1:
        {
            angle_t angles = {
                GCODE_parse_number('X', -1.0F),
                GCODE_parse_number('Y', -1.0F),
                GCODE_parse_number('Z', -1.0F)
            };
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd, // code
                &angles // the return value itself
            };
            break;
        }
        case 4:
        case 28:
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd, // code
                NULL // the return value itself
            };
            break;
        default:
        {
            const char *msg = "Unknown GCODE G%d\n";
            ret = (GCODE_ret_t) {
                true, // is_err
                cmd, // the code itself
                msg // the error message
            };
            break;
        }
    }

    cmd = (int_fast16_t) GCODE_parse_number('M', -1.0F);
    switch (cmd) {
        case 18:
            // TODO - PAUSE
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd * 10, // code
                NULL // the return value itself
            };
            break;
        case 114:
        case 280:
        {
            angle_t current_position = GCODE_get_position();
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd * 10, // code
                &current_position // the return value itself
            };
            break;
        }
        default:
        {
            const char *msg = "Unknown GCODE M%d\n";
            ret = (GCODE_ret_t) {
                true, // is_err
                cmd * 10, // the code
                msg // the error message
            };
            break;
        }
    }

    cmd = (int_fast16_t) GCODE_parse_number('I', -1.0F);
    switch (cmd) {
        case 1:
        {
            ret = (GCODE_ret_t) {
                false, // is_err
                cmd * 100, // the code
                NULL // the return value itself
            };
            break;
        }
        default:
        {
            const char *msg = "Unknown GCODE I%d\n";
            ret = (GCODE_ret_t) {
                true, // is_err
                cmd * 100, // the code
                msg // the error message
            };
            break;
        }
    }
    return ret;
}
