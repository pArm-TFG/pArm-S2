/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
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

