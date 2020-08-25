/* 
 * File: kinematics.c
 * Author: Javinator9889
 * Comments: Contains the definitions for both forward and inverse kinematics
 * Revision history: v1.0
 */

#include <math.h>
#include "kinematics.h"
#include "../utils/types.h"
#include "../utils/utils.h"

float get_radius_from_height(float height) {
    float radius = .0f;
    float v_distance = .0f;

    if (height > 111.70f) {
        v_distance = fabs(215.87f - height);
        float data = (158.8f * 158.8f) - (v_distance * v_distance);
        radius = sqrt(data) - 33.26f;
    } else {
        v_distance = fabs(height + 20.88f);
        float data = (225.0f * 225.0f) - (v_distance * v_distance);
        radius = sqrt(data) - 97.45f;
    }

    radius -= 44.5f;
    return radius;
}

char inverse_kinematics(const point_t in_cartesian, angle_t angle) {
    float xIn = .0f;
    float zIn = .0f;
    float rightAll = .0f;
    float sqrtXZ = .0f;
    float phi = .0f;

    // Create a copy of the struct
    point_t point = in_cartesian;

    float angleRot = .0f;
    float angleLeft = .0f;
    float angleRight = .0f;

    point.z += height_offset;

    zIn = (point.z - MATH_L1) / MATH_LOWER_ARM;

    float xy_length = sqrt(point.x * point.x + point.y * point.y);
    float radius = get_radius_from_height(point.z);

    if ((xy_length - front_end_offset) < radius)
        return -1;

    if (point.x < .01f)
        point.x = .01f;

    // Calculate value of theta 1 (rotation angle)
    if (point.y == 0)
        angleRot = 90.0f;
    else {
        angleRot = (point.y < 0) ?
                -atan2(point.x, point.y) * MATH_TRANS :
                180.F - atan2(point.x, point.y) * MATH_TRANS;
    }

    xIn = (point.x / sin(angleRot / MATH_TRANS) - MATH_L2 - front_end_offset)
            / MATH_LOWER_ARM;

    phi = atan2(zIn, xIn) * MATH_TRANS;
    sqrtXZ = sqrt(zIn * zIn + xIn * xIn);

    // Cosine law
    rightAll = (sqrtXZ * sqrtXZ + MATH_UPPER_LOWER * MATH_UPPER_LOWER - 1) /
            (2 * MATH_UPPER_LOWER * sqrtXZ);
    angleRight = acos(rightAll) * MATH_TRANS;

    // Calculate the value of theta 2
    rightAll = (sqrtXZ * sqrtXZ + 1 - MATH_UPPER_LOWER * MATH_UPPER_LOWER) /
            (2 * sqrtXZ);
    angleLeft = acos(rightAll) * MATH_TRANS;

    angleLeft += phi;
    angleRight -= phi;

    if (isnan(angleRot) || isnan(angleLeft) || isnan(angleRight))
        return -1;

    if (angleLeft > LOWER_ARM_MAX_ANGLE) {
        return -1;
    } else if (angleLeft < LOWER_ARM_MIN_ANGLE) {
        return -1;
    }

    if (angleRight > UPPER_ARM_MAX_ANGLE) {
        return -1;
    } else if (angleRight < UPPER_ARM_MIN_ANGLE) {
        return -1;
    }
    
    if (angleLeft + angleRight > (180 - LOWER_UPPER_MIN_ANGLE)) {
        //angleLeft = 180  - LOWER_UPPER_MIN_ANGLE - angleRight;
        return -1;
    }

    if ((180 - angleLeft - angleRight) > LOWER_UPPER_MAX_ANGLE) {
        //angleRight = 180 - LOWER_UPPER_MAX_ANGLE - angleLeft;
        return -1;
    }


    angleRot = constrain(angleRot, 0.00f, 180.00f);
    angleLeft = constrain(angleLeft, 0.00f, 180.00f);
    angleRight = constrain(angleRight, 0.00f, 180.00f);


    angle.theta0 = angleRot;
    angle.theta1 = angleLeft;
    angle.theta2 = angleRight;

    return 0;
}
