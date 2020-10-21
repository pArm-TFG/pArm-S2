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
 * File: kinematics.h
 * Author: Javinator9889
 * Comments: Contains the definitions for both forward and inverse kinematics
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef KINEMATICS_H
#define	KINEMATICS_H

#include <stdbool.h>
#include "../utils/types.h"

/**
 * Utility funciton used to check if the given angle and point are OK.
 * 
 * @param angle the input angle. Can be NULL.
 * @param point the input point. Can be NULL.
 * @return true or false indicating if the input params are OK.
 */
bool check_constraints_ok(angle_t* angle, point_t* point);

/**
 * Performs the inverse kinematics for the pArm.
 * 
 * @param in_cartesian the input point.
 * @param angle a pointer in which the obtained angle will be stored.
 * @return EXIT_SUCCESS if everything went OK or EXIT_FAILURE in other case.
 */
char inverse_kinematics(point_t in_cartesian, angle_t *angle);

/**
 * Performs the forward kinematics for the pArm.
 * 
 * @param in_angle the input angle.
 * @param position a pointer in which the obtained position will be stored.
 * @return EXIT_SUCCESS if everything when OK or EXIT_FAILURE in other case.
 */
char forward_kinematics(angle_t in_angle, point_t *position);


#endif	/* KINEMATICS_H */

