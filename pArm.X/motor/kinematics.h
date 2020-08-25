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

#define DEFAULT_NORMAL_HEIGHT	74.55
#define DEFAULT_NORMAL_FRONT	56.65

#define DEFAULT_LASER_HEIGHT	51.04
#define DEFAULT_LASER_FRONT		64.4

#define DEFAULT_3DPRINT_HEIGHT	74.43
#define DEFAULT_3DPRINT_FRONT	56.5

#define DEFAULT_PEN_HEIGHT	74.43
#define DEFAULT_PEN_FRONT	69.5 	//92.8	

#define MATH_PI 			3.141592653589793238463
#define MATH_TRANS  		57.2958    
#define MATH_L1 			106.6	//90.00	
#define MATH_L2 			13.2		//21.17	
#define MATH_LOWER_ARM 		142.07	//148.25	
#define MATH_UPPER_ARM 		158.81	//158.8	//160.2 	
//#define MATH_FRONT_HEADER 	69//(29.4+38.6)//43.75)	//25.00// the distance between wrist to the front point we use
#define MATH_UPPER_LOWER 	MATH_UPPER_ARM/MATH_LOWER_ARM

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

#define LOWER_ARM_MAX_ANGLE      135.6
#define LOWER_ARM_MIN_ANGLE      0
#define UPPER_ARM_MAX_ANGLE      119.9
#define UPPER_ARM_MIN_ANGLE      0
#define LOWER_UPPER_MAX_ANGLE    151
#define LOWER_UPPER_MIN_ANGLE    10

#include "../utils/types.h"

char inverse_kinematics(const point_t in_cartesian, angle_t angle);
char forward_kinematics(point_t position);


#endif	/* KINEMATICS_H */

