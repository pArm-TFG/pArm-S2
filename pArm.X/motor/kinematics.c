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

bool check_angle_constraints(angle_t *angle) {
    return ((angle->theta0 > LOWER_UPPER_MIN_ANGLE) &&
            (angle->theta0 < LOWER_UPPER_MAX_ANGLE) &&
            (angle->theta1 > LOWER_ARM_MIN_ANGLE) &&
            (angle->theta1 < LOWER_ARM_MAX_ANGLE) &&
            (angle->theta2 > UPPER_ARM_MIN_ANGLE) &&
            (angle->theta2 < UPPER_ARM_MAX_ANGLE));
}

bool check_constraints_ok(angle_t *angle, point_t *point) {
    bool res = true;
    if (isnan(angle->theta0) || isnan(angle->theta1) || isnan(angle->theta2) 
            || isnan(point->x) || isnan(point->y) || isnan(point->z))
        return false;
    
    if (angle->theta0 > DEG_151)
        res = false;
    if (angle->theta1 > DEG_135)
        res = false;
    if (angle->theta2 > DEG_120)
        res = false;
    if (sqrtl(powl(point->x, 2) + powl(point->y, 2) + powl(point->z, 2)) > 261.0F)
        res = false;
    if (angle->theta2 > (angle->theta1 + DEG_55))
        res = false;
    if ((point->x > -53.0F && point->x < 53.0F) &&
            (point->z < 0.0F) &&
            (point->y > -53.0F && point->y < 53.0F))
        res = false;
    
    return res;
}

char inverse_kinematics(point_t in_cartesian, angle_t* angle) {
#define AL2     (ARM_LOWER_ARM * ARM_LOWER_ARM)
#define XYZ2    (lxyz * lxyz)
#define AU2     (ARM_UPPER_ARM * ARM_UPPER_ARM)

    double64_t theta_0 = atan2l(in_cartesian.x, in_cartesian.y);
    double64_t xyz = powl(in_cartesian.x, 2) +
            powl(in_cartesian.y, 2) +
            powl(in_cartesian.z, 2);
    if (isnan(xyz))
        return EXIT_FAILURE;
    double64_t lxyz = sqrtl(xyz);
    double64_t theta_1 = acosl((-AL2 - XYZ2 + AU2) / (-2 * ARM_LOWER_ARM * lxyz));
    double64_t theta_2 = acosl((-AL2 -AU2 + XYZ2) / (-2 * ARM_LOWER_ARM * ARM_UPPER_ARM));
    double64_t phi = atan2l(in_cartesian.z, sqrtl(powl(in_cartesian.x, 2) + powl(in_cartesian.y, 2)));
    if (isnan(theta_0) || isnan(theta_1) || isnan(theta_2) || isnan(phi))
        return EXIT_FAILURE;
    
    theta_1 += phi;
    theta_1 -= DEG_135;
    
    angle->theta0 = theta_0;
    angle->theta1 = theta_1;
    angle->theta2 = theta_2;
    
    return EXIT_SUCCESS;
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
