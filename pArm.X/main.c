/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */

#include "utils/defs.h"
#include "pragmas.h"
#include <xc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef USE_CUSTOM_PRINTF
#include "printf/printf.h"
#endif
#include "utils/types.h"
#include "utils/uart.h"
#include "init.h"
#include "interrupts.h"
#include "utils/time.h"
#include "rsa/rsa.h"
#include "motor/motor.h"
#include "arm/planner.h"
#include "gcode/gcode.h"

rsa_t *RSA_key = NULL;
bool message_received = false;
char *order_buffer = NULL;
uint16_t order_chars = 0;


void setup(void);
void loop(void);
char check_motor_status(void);

int main(void) {
    setup();
    while (true) {
        loop();
    }
    return 0;
}

inline void setup(void) {
    // Initialize different system modules
    system_initialize();

    // Calibrate the motors. If someone returns
    // not OK, stop execution until rebooted
    // and notify turning on an LED
    if (check_motor_status() == EXIT_FAILURE) {
        // Turn on LEDs
        PORTBbits.RB5 = 1;
        PORTBbits.RB6 = 1;
        PORTBbits.RB7 = 1;
        while (true) {
            // I2 stands for motor failure
            printf("I2\n");
            delay_ms(1000);
        }
    }
    rsa_t key = RSA_keygen();
    RSA_key = &key;
}

inline void loop(void) {
    while (!message_received);
    message_received = false;
    GCODE_ret_t ret = GCODE_process_command(order_buffer);
    if (ret.is_err) {
        printf("Fail\n");
    }
}

inline char check_motor_status(void) {
    char motor_status = MOTOR_calibrate(motors.base_motor);
    if (motor_status == EXIT_FAILURE)
        return motor_status;
    motor_status = MOTOR_calibrate(motors.lower_arm);
    if (motor_status == EXIT_FAILURE)
        return motor_status;
    motor_status = MOTOR_calibrate(motors.upper_arm);

    return motor_status;
}
