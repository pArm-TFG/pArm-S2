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

