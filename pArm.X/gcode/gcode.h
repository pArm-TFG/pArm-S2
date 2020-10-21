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
 * File: gcode.h
 * Author: Javinator9889
 * Comments: GCode interpreter
 * Revision history: v1.0
 */

#ifndef GCODE_H
#define	GCODE_H

#include <stdint.h>
#include "../utils/types.h"
#include "../utils/uart.h"

/**
 * With the given order, process the command sent to the device.
 * 
 * @param order the order to be parsed.
 * @return GCODE_ret_t containing the parsed command.
 */
GCODE_ret_t GCODE_process_command(volatile order_t *order);

#endif	/* GCODE_H */

