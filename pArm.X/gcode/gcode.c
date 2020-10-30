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
#include "../utils/buffer.h"
#include "../printf/io.h"

static buffer_t *gcode_buffer = NULL;

/**
 * With the given input code and the buffer, iterates through the latest one
 * until the code is found or returns the default value.
 * 
 * @param code - the code to search.
 * @param ret - return value if not found.
 * @return the number after the given code or the given return value if not found.
 */
double64_t GCODE_parse_number(char code, double64_t ret) {
    char *token;
    char *ptr;
    char *copy = (char *) malloc(gcode_buffer->bsize);
    double64_t value_if_missing = ret;
    strcpy(copy, gcode_buffer->buffer);
    for (ptr = copy; ret == value_if_missing; ptr = NULL) {
        token = strtok(ptr, " ");
        if (token == NULL) break;
        if (token[0] == code) ret = atof(token + 1);
    }
    free(copy);
    return ret;
}

/**
 * Safely clears the GCODE structures.
 * 
 * @param ret - the return code to be returned after finishing.
 * @return ret
 */
static inline GCODE_ret_t GCODE_finish(GCODE_ret_t ret) {
    BUFFER_free(gcode_buffer);
    return ret;
}

GCODE_ret_t GCODE_process_command(volatile order_t *order) {
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tParsing order '%s'\n", order->order_buffer->buffer);
#endif
    GCODE_ret_t ret; // = {false, -1, NULL};

    if (gcode_buffer == NULL) {
        gcode_buffer = BUFFER_create(order->order_buffer->size);
    }

    if (gcode_buffer->size != order->order_buffer->size) {
        BUFFER_update_size(gcode_buffer, order->order_buffer->size);
    }
    strcpy(gcode_buffer->buffer, order->order_buffer->buffer);
    int_fast16_t cmd = (int_fast16_t) GCODE_parse_number('G', -1.0F);
    switch (cmd) {
            // G0 X1.234 Y1.234 Z1.234
            // receives a position by the given
            // coordinates in XYZ.
            // If coordinates are missing,
            // throw an error
        case 0:
        {
            point_t *position = (point_t *) malloc(sizeof(point_t));
            position->x = GCODE_parse_number('X', LDBL_MIN);
            position->y = GCODE_parse_number('Y', LDBL_MIN);
            position->z = GCODE_parse_number('Z', LDBL_MIN);
            if (position->x == LDBL_MIN ||
                    position->y == LDBL_MIN ||
                    position->z == LDBL_MIN) {
#ifdef DEBUG_ENABLED
                printf("[ERROR]\tCoordinates missing for GCODE G0!\n");
#endif
                ret = (GCODE_ret_t){
                    true, // is_err
                    cmd,
                    NULL
                };
            } else {
                ret = (GCODE_ret_t){
                    false, // is_err
                    cmd, // code
                    position // the return value itself
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
            angle_t *angles = (angle_t *) malloc(sizeof(angle_t));
            angles->theta0 = GCODE_parse_number('X', LDBL_MIN);
            angles->theta1 = GCODE_parse_number('Y', LDBL_MIN);
            angles->theta2 = GCODE_parse_number('Z', LDBL_MIN);
            if (angles->theta0 == LDBL_MIN &&
                    angles->theta1 == LDBL_MIN &&
                    angles->theta2 == LDBL_MIN) {
#ifdef DEBUG_ENABLED
                printf("[ERROR]\tAngles missing for GCODE G1!\n");
#endif
                ret = (GCODE_ret_t){
                    true, // is_err
                    cmd,
                    NULL
                };
            } else {
                ret = (GCODE_ret_t){
                    false, // is_err
                    cmd, // code
                    angles // the return value itself
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
#ifdef DEBUG_ENABLED
            if (cmd == -1)
                printf("[INFO]\tGCODE type 'G' not found\n");
            else
                printf("[ERROR]\tUnknown GCODE G%d\n", cmd);
#endif
            ret = (GCODE_ret_t){
                true, // is_err
                cmd, // the code itself
                NULL // the error message
            };
            break;
        }
    }

    // GCODE found so quit and return value
    if (cmd != -1)
        return GCODE_finish(ret);

    cmd = (int_fast16_t) GCODE_parse_number('M', -1.0F);
    switch (cmd) {
            // GCODE M1 - unconditional stop
            // When this command is received, the arm
            // must stop moving and keep current position
        case 1:
            // GCODE M114 - get current position in XYZ
            // By default, obtains the position in angular
            // coordinates and leaves the conversion to 
            // main orchestrator
        case 114:
            // GCODE M280 - get current position in angles
        case 280:
        {
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 10, // code
                NULL // the return value itself
            };
            break;
        }
            // The input GCODE is not recognized
            // (not implemented)
        default:
        {
#ifdef DEBUG_ENABLED
            if (cmd == -1)
                printf("[INFO]\tGCODE type 'M' not found\n");
            else
                printf("[ERROR]\tUnknown GCODE M%d\n", cmd);
#endif
            ret = (GCODE_ret_t){
                true, // is_err
                cmd * 10, // the code
                NULL // the error message
            };
            break;
        }
    }

    // GCODE found so quit and return value
    if (cmd != -1)
        return GCODE_finish(ret);

    cmd = (int_fast16_t) GCODE_parse_number('I', -1.0F);
    switch (cmd) {
            // GCODE I1 - custom command for sending RSA public key
            // When received, the main orchestrator must send
            // both modulus (n) and public exponent (e)
            // so the other system can decrypt our messages.
        case 1:
        {
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 100, // the code
                NULL // the return value itself
            };
            break;
        }
            // All the following must have the random message encrypted.
            // GCODE I5 - received the unsigned message
            // when verifying
        case 5:
            // GCODE I6 - generate new RSA keys
        case 6:
            // GCODE I7 - received heartbeat so we know
            // the trusted device is still alive
        case 7:
        {
            size_t size = (size_t) ((gcode_buffer->size - 3) * sizeof (char));
            char *msg = (char *) malloc(size);
            strncpy(msg, gcode_buffer->buffer + 3, size);
            ret = (GCODE_ret_t){
                false, // is_err
                cmd * 100, // the code
                msg // the msg
            };
            break;
        }
            // The input GCODE is not recognized
            // (not implemented)
        default:
        {
#ifdef DEBUG_ENABLED
            if (cmd == -1)
                printf("[INFO]\tGCODE type 'I' not found\n");
            else
                printf("[ERROR]\tUnknown GCODE I%d\n", cmd);
#endif
            ret = (GCODE_ret_t){
                true, // is_err
                cmd * 100, // the code
                NULL // the msg
            };
            break;
        }
    }
    return GCODE_finish(ret);
}
