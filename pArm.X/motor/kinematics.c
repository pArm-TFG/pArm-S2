/*
 * Copyright (C) 2020 - present | pArm-S2 by Javinator9889
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
 * File: kinematics.c
 * Author: Javinator9889
 * Comments: Contains the definitions for both forward and inverse kinematics
 * Revision history: v1.0
 */

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "kinematics.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../arm_config.h"
#ifdef DEBUG_ENABLED
#include "../printf/io.h"
#endif

void do_forward_kinematics(
        const angle_t angle,
        point_t *res,
        const double64_t a1,
        const double64_t a2,
        const double64_t a3,
        const double64_t d1,
        const double64_t Tx,
        const double64_t Tz
        ) {
    res->x = (a2 * cosl(angle.theta1) + a3 * cosl(angle.theta1 - angle.theta2) + d1) 
            * cosl(angle.theta0) + Tx;
    res->y = (a2 * cosl(angle.theta1) + a3 * cosl(angle.theta1 - angle.theta2) + d1) 
            * sinl(angle.theta0);
    res->z = a1 + (a2 * sinl(angle.theta1)) + (a3 * sinl(angle.theta1 - angle.theta2)) 
            - Tz;
}

bool check_constraints_ok(angle_t *angle, point_t *point) {
    bool res = true;
    if (__isnan(angle->theta0) || __isnan(angle->theta1) || __isnan(angle->theta2) 
            || __isnan(point->x) || __isnan(point->y) || __isnan(point->z))
        return false;

    if (angle->theta0 > DEG_151) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tTheta 0 value bigger than 151D\n");
#endif
        res = false;
    }
    if (angle->theta1 > DEG_135) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tTheta 1 value bigger than 135D\n");
#endif
        res = false;
    }
    if (angle->theta2 > DEG_120) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tTheta 2 value bigger than 120D\n");
#endif
        res = false;
    }
    if (sqrtl(powl(point->x, 2) + powl(point->y, 2)) > 261.0F) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tLength sqrt(x2 + y2) > 261!\n");
#endif
        res = false;
    }
    if (angle->theta2 > (angle->theta1 + DEG_55)) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tPhysical structure limit\n");
#endif
        res = false;
    }
    if ((point->x > -53.0F && point->x < 53.0F) &&
            (point->z < 0.0F) &&
            (point->y > -53.0F && point->y < 53.0F)) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tEnd-effector colliding with arm base!\n");
#endif
        res = false;
    }

    return res;
}

char inverse_kinematics(point_t in_cartesian, angle_t* angle) {
#define AL2     (ARM_LOWER_ARM * ARM_LOWER_ARM)
#define AU2     (ARM_UPPER_ARM * ARM_UPPER_ARM)

    double64_t theta_0 = atan2l(in_cartesian.x, in_cartesian.y);
    double64_t xyz = powl(in_cartesian.x, 2) +
            powl(in_cartesian.y, 2) +
            powl(in_cartesian.z, 2);
    double64_t lxyz = sqrtl(xyz);
    double64_t theta_1 = acosl((-AL2 - xyz + AU2) / (-2 * ARM_LOWER_ARM * lxyz));
    double64_t theta_2 = acosl((-AL2 - AU2 + xyz) / (-2 * ARM_LOWER_ARM * ARM_UPPER_ARM));
    double64_t phi = atan2l(in_cartesian.z, sqrtl(powl(in_cartesian.x, 2) + powl(in_cartesian.y, 2)));

    theta_1 += phi;
    theta_1 = DEG_135 - theta_1;

    angle->theta0 = theta_0;
    angle->theta1 = theta_1;
    angle->theta2 = theta_2;

    if (check_constraints_ok(angle, &in_cartesian) == true)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}

char forward_kinematics(angle_t in_angle, point_t *position) {
    do_forward_kinematics(
            in_angle,
            position,
            ARM_BASE_HEIGHT,
            ARM_LOWER_ARM,
            ARM_UPPER_ARM,
            ARM_BASE_DEVIATION,
            front_end_offset,
            -ARM_BASE_DEVIATION
            );
    return check_constraints_ok(&in_angle, position)
            ? EXIT_SUCCESS
            : EXIT_FAILURE;
}
