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
#include <math.h>
#include <float.h>
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
#include "rsa/rand.h"

rsa_t *RSA_key = NULL;
bool message_received = false;
char *order_buffer = NULL;
uint16_t order_chars = 0;
volatile bool trusted_device = false;
int_fast64_t rnd_message;
double64_t motor_movement_finished_time = LDBL_MAX;
time_t last_beat = UINT64_C(0);

void setup(void);
void loop(void);
char check_motor_status(void);
void do_handshake(void);
void handle_order(void);
void do_movement(double64_t expected_time);
void beat(int_fast64_t encrypted_msg);

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
    TIME_set_time(UINT64_C(0));

    // Initialize RAND module
    RAND_init();

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
            printf("J2\n");
            delay_ms(500);
        }
    }
    // Initialize RAND seed before generating the new keys
    RAND_init_seed();

    rsa_t key = RSA_keygen();
    RSA_key = &key;
}

inline void loop(void) {
    if (!trusted_device) {
        do_handshake();
    }
    if (message_received) {
        message_received = false;
        GCODE_ret_t ret = GCODE_process_command(order_buffer);
        switch (ret.code) {
                // G0
            case 0:
            {
                point_t *position = (point_t *) ret.gcode_ret_val;
                double64_t expected_time = PLANNER_move_xyz(*position);
                do_movement(expected_time);
                break;
            }
                // G1
            case 1:
            {
                angle_t *angles = (angle_t *) ret.gcode_ret_val;
                double64_t expected_time = PLANNER_move_angle(*angles);
                do_movement(expected_time);
                break;
            }
                // G28
            case 28:
            {
                double64_t expected_time = PLANNER_go_home();
                do_movement(expected_time);
                break;
            }
                // M1
            case 10:
            {
                PLANNER_stop_moving();
                break;
            }
                // M114
            case 1140:
            {
                point_t *position = PLANNER_get_position();
                printf("G0 X%lf Y%lf Z%lf\n", 
                        position->x, 
                        position->y, 
                        position->z);
                break;
            }
                // M280
            case 2800:
            {
                angle_t *position = PLANNER_get_angles();
                printf("G1 X%lf Y%lf Z%lf\n",
                        position->theta0,
                        position->theta1,
                        position->theta2);
                break;
            }
                // I6
            case 600:
            {
                int_fast64_t encrypted_msg = (int_fast64_t) ret.gcode_ret_val;
                int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
                if (msg == rnd_message) {
                    trusted_device = false;
                    *RSA_key = RSA_keygen();
                    rnd_message = UINT64_C(0);
                } else {
                    printf("J6\n");
                }
                break;
            }
                // I7
            case 700:
            {
                int_fast64_t encrypted_msg = (int_fast64_t) ret.gcode_ret_val;
                beat(encrypted_msg);
                break;
            }
            // Invalid GCODE found
            default:
            {
                printf("J3\n");
                break;
            }
        }
    }
    if (trusted_device) {
        // If last beat happened at least 1 second ago
        // untrust the device and send 'J6' for informing
        if ((TIME_now() - last_beat) >= UINT64_C(1000)) {
            printf("J8\n");
            trusted_device = false;
            *RSA_key = RSA_keygen();
            rnd_message = UINT64_C(0);
        }
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

inline void do_handshake(void) {
    while (!message_received);
    message_received = false;
    GCODE_ret_t ret = GCODE_process_command(order_buffer);
    switch (ret.code) {
            // I1
        case 100:
        {
            printf("I2 %lld\n", RSA_key->n);
            printf("I3 %lld\n", RSA_key->e);
            rnd_message = RAND(10007LL, 104729LL);
            int_fast64_t signed_message = RSA_sign(rnd_message, RSA_key);
            printf("I4 %lld\n", signed_message);
            break;
        }
            // I5 with decrypted msg
        case 500:
        {
            int_fast64_t encrypted_msg = (int_fast64_t) ret.gcode_ret_val;
            int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
            if (msg == rnd_message) {
                printf("I5\n");
                trusted_device = true;
                last_beat = TIME_now();
            } else {
                printf("J6\n");
                trusted_device = false;
            }
            break;
        }
        default:
        {
            // Untrusted device
            printf("J8\n");
            break;
        }
    }
}

inline void do_movement(double64_t expected_time) {
    printf("J1 %lf\n", expected_time);
    motor_movement_finished_time = TIME_now_us() + expected_time;
}

inline void beat(int_fast64_t encrypted_msg) {
    int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
    if (msg == rnd_message) {
        last_beat = TIME_now();
    }
}
