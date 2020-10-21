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
 * File: arm_config.h
 * Author: Javinator9889
 * Comments: The arm configuration that may be used across the application
 * Revision history: v1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
/**
 * Different configurations are defined in this document for the pArm
 * manipulator. As intended, multiple end-effectors can be attached,
 * so here are the definitions for all of them (got from both GitHub
 * UFACTORY project and custom pieces definition)
 */
#ifndef ARM_CONFIG_H
#define	ARM_CONFIG_H

#define DEFAULT_NORMAL_HEIGHT	74.55
#define DEFAULT_NORMAL_FRONT	56.65

#define DEFAULT_LASER_HEIGHT	51.04
#define DEFAULT_LASER_FRONT		64.4

#define DEFAULT_3DPRINT_HEIGHT	74.43
#define DEFAULT_3DPRINT_FRONT	56.5

#define DEFAULT_PEN_HEIGHT      74.43
#define DEFAULT_PEN_FRONT       69.5

#define ARM_BASE_HEIGHT         133.2
#define ARM_BASE_DEVIATION      13.2
#define ARM_LOWER_ARM           142.07
#define ARM_UPPER_ARM           158.81
#define ARM_UPPER_LOWER         (ARM_UPPER_ARM / ARM_LOWER_ARM)

#ifdef NORMAL_MODE
#define height_offset DEFAULT_NORMAL_HEIGHT
#define front_end_offset DEFAULT_NORMAL_FRONT
#elif defined(PEN_MODE)
#define height_offset DEFAULT_PEN_HEIGHT
#define front_end_offset DEFAULT_PEN_FRONT
#elif defined(LASER_MODE)
#define height_offset DEFAULT_LASER_HEIGHT
#define front_end_offset DEFAULT_LASER_FRONT
#else
#define height_offset DEFAULT_NORMAL_HEIGHT
#define front_end_offset DEFAULT_NORMAL_FRONT
#endif

#define LOWER_ARM_MAX_ANGLE     135.6
#define LOWER_ARM_MIN_ANGLE     0.0
#define UPPER_ARM_MAX_ANGLE     119.9
#define UPPER_ARM_MIN_ANGLE     0.0
#define LOWER_UPPER_MAX_ANGLE   151.0
#define LOWER_UPPER_MIN_ANGLE   10.0

// https://github.com/UPM-Robotics/uarm/blob/master/docs/robot-information/
#define ARM_MAX_X_LENGTH        346F
#define ARM_MAX_Z_HEIGHT        360.6F
#define ARM_MIN_X_LENGTH        87F
#define ARM_MIN_Z_HEIGHT        ARM_BASE_HEIGHT

#endif	/* ARM_CONFIG_H */

