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

void GCODE_move_to(point_t position) {
    return;
}

point_t GCODE_get_position(void) {
    point_t position = {.0f, .0f, .0f};
    return position;
}

void GCODE_pause(void) {
    return;
}

/*float GCODE_parser_instruction(char code) {
    char *ptr = GCODE_BUFFER;
    while ((long) ptr > 1 && (*ptr) && (long) ptr < (long) GCODE_BUFFER + cLength) {
        
    }
}*/

float GCODE_parse_number(char code, float val) {
    char *ptr = GCODE_BUFFER;
    while ((long) ptr > 1 && (*ptr) && (long) ptr < (long) GCODE_BUFFER + cLength) {
        if (*ptr == code) return atof(ptr + 1);
        ptr = strchr(ptr, ' ') + 1;
    }
    return val;
}

void GCODE_process_command(const char* command) {

    foreach(char, code, command) {
        if (cLength < (MAX_BUFFER_LENGTH - 1))
            GCODE_BUFFER[cLength++] = code;
        if (code == "\n" || code == "\r") {
            GCODE_BUFFER[cLength] = 0;
            break;
        }
    }
    uint8_t cmd = GCODE_parse_number('G', -1);
    switch (cmd) {
        case 0:
        case 1:
        {
            point_t position = {
                GCODE_parse_number('X', -1),
                GCODE_parse_number('Y', -1),
                GCODE_parse_number('Z', -1)
            };
            GCODE_move_to(position);
            break;
        }
        case 2:
        case 3:
            // TODO - ARC
            break;
        case 4:
            // TODO - PAUSE
            break;
        case 28:
        {
            // TODO - Home
            point_t position = {
                GCODE_parse_number('X', -1),
                GCODE_parse_number('Y', -1),
                GCODE_parse_number('Z', -1)
            };
            if ((position.x == 0 && position.y == 0 && position.z == 0) ||
                    ((position.x == -1 && position.y == -1 && position.z == -1)))
                PLANNER_go_home();
            if (position.x == 0) {
                MOTOR_home(motors.lower_arm);
            }
            if (position.y == 0) {
                MOTOR_home(motors.base_motor);
            }
            if (position.z == 0) {
                MOTOR_home(motors.upper_arm);
            }
            break;
        }
        default:
            break;
    }

    cmd = GCODE_parse_number('M', -1);
    switch (cmd) {
        case 18:
            GCODE_pause();
            break;
        case 114:
        {
            // TODO - manage position
            point_t current_position = GCODE_get_position();
            break;
        }
        default:
            break;
    }
    
    cmd = GCODE_parse_number('I', -1);
    switch (cmd) {
        case -1:
            break;
        default:
            break;
    }
}
