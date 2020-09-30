/* 
 * File: kinematics.c
 * Author: Javinator9889
 * Comments: Contains the definitions for both forward and inverse kinematics
 * Revision history: v1.0
 */

#include <math.h>
#include "kinematics.h"
#include "../utils/defs.h"
#include "../utils/types.h"
#include "../utils/utils.h"
#include "../arm_config.h"

double64_t get_radius_from_height(double64_t height) {
    double64_t radius = .0f;
    double64_t v_distance = .0f;

    if (height > 111.70f) {
        v_distance = fabsl(215.87f - height);
        double64_t data = (158.8f * 158.8f) - (v_distance * v_distance);
        radius = sqrtl(data) - 33.26f;
    } else {
        v_distance = fabsl(height + 20.88f);
        double64_t data = (225.0f * 225.0f) - (v_distance * v_distance);
        radius = sqrtl(data) - 97.45f;
    }

    radius -= 44.5f;
    return radius;
}

inline double64_t **forward_kinematics_matrix(
        const angle_t angle,
        const double64_t a1,
        const double64_t a2,
        const double64_t a3,
        const double64_t d1,
        const double64_t Tx,
        const double64_t Tz
        ) {

    return ((double64_t[4][4]) {
        {
            cosl(angle.theta0) * cosl(angle.theta1 - angle.theta2),
            sinl(angle.theta1 - angle.theta2) * cosl(angle.theta0),
            -sinl(angle.theta0),
            (a2 * cosl(angle.theta1) + a3 * cosl(angle.theta1 - angle.theta2) + d1) * cosl(angle.theta0) + Tx
        },
        {
            sinl(angle.theta0) * cosl(angle.theta1 - angle.theta2),
            sinl(angle.theta1 - angle.theta2) * sinl(angle.theta0),
            cosl(angle.theta0),
            (a2 * cosl(angle.theta1) + a3 * cosl(angle.theta1 - angle.theta2) + d1) * sinl(angle.theta0)
        },
        {
            sinl(angle.theta1 - angle.theta2),
            -cosl(angle.theta1 - angle.theta2),
            0,
            a1 + a2 * sinl(angle.theta1) + a3 * sinl(angle.theta1 - angle.theta2) - Tz
        },
        {.0F, .0F, .0F, 1.0F}
    });
}

inline void check_angle_constraints(angle_t *angle) {
    angle->theta0 = constraint(angle->theta0, LOWER_UPPER_MIN_ANGLE, LOWER_UPPER_MAX_ANGLE);
    angle->theta1 = constraint(angle->theta1, LOWER_ARM_MIN_ANGLE, LOWER_ARM_MAX_ANGLE);
    angle->theta2 = constraint(angle->theta2, UPPER_ARM_MIN_ANGLE, UPPER_ARM_MAX_ANGLE);
}

char inverse_kinematics(const point_t in_cartesian, angle_t *angle) {
    double64_t xIn = .0f;
    double64_t zIn = .0f;
    double64_t rightAll = .0f;
    double64_t sqrtXZ = .0f;
    double64_t phi = .0f;

    // Create a copy of the struct
    point_t point = in_cartesian;

    double64_t angleRot = .0f;
    double64_t angleLeft = .0f;
    double64_t angleRight = .0f;

    point.z += height_offset;

    zIn = (point.z - ARM_BASE_HEIGHT) / ARM_LOWER_ARM;

    double64_t xy_length = sqrtl(point.x * point.x + point.y * point.y);
    double64_t radius = get_radius_from_height(point.z);

    if ((xy_length - front_end_offset) < radius)
        return -1;

    if (point.x < .01f)
        point.x = .01f;

    // Calculate value of theta 1 (rotation angle).
    // As we are using 'atan2', there is no need to
    // use specific cases (y > 0, y < 0, y = 0) but just
    // do pi - atan2(x, y);
    angleRot = MATH_PI - atan2l(point.x, point.y);

    xIn = (point.x / sinl(angleRot) - ARM_BASE_DEVIATION - front_end_offset)
            / ARM_LOWER_ARM;

    phi = atan2l(zIn, xIn);
    sqrtXZ = sqrtl(zIn * zIn + xIn * xIn);

    // Cosine law
    rightAll = (sqrtXZ * sqrtXZ + ARM_UPPER_LOWER * ARM_UPPER_LOWER - 1) /
            (2 * ARM_UPPER_LOWER * sqrtXZ);
    angleRight = acosl(rightAll);

    // Calculate the value of theta 2
    rightAll = (sqrtXZ * sqrtXZ + 1 - ARM_UPPER_LOWER * ARM_UPPER_LOWER) /
            (2 * sqrtXZ);
    angleLeft = acosl(rightAll);

    angleLeft += phi;
    angleRight -= phi;

    if (isnan(angleRot) || isnan(angleLeft) || isnan(angleRight))
        return -1;

    angleRot = constraint(angleRot,
            (MATH_PI * LOWER_UPPER_MIN_ANGLE) / 180.0F,
            (MATH_PI * LOWER_UPPER_MAX_ANGLE) / 180.0F);
    angleLeft = constraint(angleLeft,
            (MATH_PI * LOWER_ARM_MIN_ANGLE) / 180.0F,
            (MATH_PI * LOWER_ARM_MAX_ANGLE) / 180.0F);
    angleRight = constraint(angleRight,
            (MATH_PI * UPPER_ARM_MIN_ANGLE) / 180.0F,
            (MATH_PI * UPPER_ARM_MAX_ANGLE) / 180.0F);


    angle->theta0 = angleRot;
    angle->theta1 = angleLeft;
    angle->theta2 = angleRight;

    return 0;
}

char forward_kinematics(const angle_t in_angle, point_t *position) {
    angle_t angle = in_angle;
    check_angle_constraints(&angle);

    const double64_t **fk_matrix = forward_kinematics_matrix(
            angle,
            ARM_BASE_HEIGHT,
            ARM_LOWER_ARM,
            ARM_UPPER_ARM,
            ARM_BASE_DEVIATION,
            front_end_offset,
            -ARM_BASE_DEVIATION
            );
    position->x = fk_matrix[0][3];
    position->y = fk_matrix[1][3];
    position->z = fk_matrix[2][3];

    return 1;
}
