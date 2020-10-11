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
#include <p33EP512GM604.h>
#include "printf/io.h"
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
#include "sync/barrier.h"

/**
 * Global program RSA key that will be used for securing communications.
 * @type rsa_t
 * @see rsa_t
 */
rsa_t *RSA_key = NULL;

/**
 * Global order container for managing the received orders
 * from UART.
 * 
 * @see order_t
 */
static volatile order_t *order = NULL;

/**
 * Flag active when the handshake has been successful.
 * If there is any kind of error during the message exchange
 * the device will be marked as untrusted.
 */
volatile bool trusted_device = false;

/**
 * Application's random message used for authoring the remote
 * device. When the host changes this message is destroyed.
 */
static int_fast64_t rnd_message;

/**
 * 
 */
static double64_t motor_movement_finished_time = LDBL_MAX;
static time_t last_beat = 0ULL;
barrier_t *barrier;

void setup(void);
void loop(void);
char check_motor_status(void);
void do_handshake(void);
void handle_order(void);
void do_movement(double64_t expected_time);
void beat(int_fast64_t encrypted_msg);

/*void putch(char character) {
    while (!IFS0bits.U1TXIF);
    U1TXREG = character;
}*/

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
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tStarting system setup\n");
#endif
    PORTBbits.RB5 = 1;
    PORTBbits.RB6 = 1;
    PORTBbits.RB7 = 1;
    TIME_init();
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tTime set to 0. Starting count...\n");

    printf("[SETUP]\tAllocating pointer to order\n");
#endif
    order = (order_t *) malloc(sizeof(order_t));
    if (order == NULL) {
#ifdef DEBUG_ENABLED
        printf("[ERROR]\tFailed to initialize order_t!\n");
#endif
        order_t ptr = {
            false, NULL
        };
        order = &ptr;
    }
    order->message_received = false;
    order->order_buffer = NULL;
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tInitializing UART RX\n");
#endif
    U1RX_Init(order);

    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;

#ifdef DEBUG_ENABLED
    printf("[SETUP]\tChecking motor status...\n");
#endif
    // Calibrate the motors. If someone returns
    // not OK, stop execution until rebooted
    // and notify turning on an LED
    if (check_motor_status() == EXIT_FAILURE) {
#ifdef DEBUG_ENABLED
        printf("[SETUP]\tMotor failure!\n");
#endif
        bool led_on = true;
        while (true) {
            // Switch on LEDs
            PORTBbits.RB5 = led_on;
            PORTBbits.RB6 = led_on;
            PORTBbits.RB7 = led_on;
            led_on = !led_on;
            // J2 stands for motor failure
            printf("J2\n");
            delay_ms(500);
        }
    }
#ifdef DEBUG_ENABLED
    printf("[SETUP]\tInitializing RAND seed\n");
#endif
    // Initialize RAND seed before generating the new keys
    RAND_init();
    RAND_init_seed();

#ifdef DEBUG_ENABLED
    printf("[SETUP]\tCreating barrier for motors\n");
#endif
    barrier = BARRIER_create(MAX_MOTORS - 1);
    PLANNER_init(barrier);
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tFinished setup!\n");
#endif
}

inline void loop(void) {
#ifndef CLI_MODE
    if (!trusted_device) {
        printf("[DEBUG]\tDevice not trusted... Waiting I1\n");
        do_handshake();
    }
#endif
    if (order->message_received) {
        order->message_received = false;
        GCODE_ret_t ret = GCODE_process_command(order);
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
                printf("M1\n");
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
                    rnd_message = 0LL;
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
        free(order->order_buffer);
    }
    if (BARRIER_all_done(barrier)) {
        // Notify all motors have finished their movement
        printf("J21\n");
        // and clear barrier interrupt flag
        BARRIER_clr(barrier);
    }
#ifndef CLI_MODE
    if (trusted_device) {
        // If last beat happened at least 1 second ago
        // untrust the device and send 'J6' for informing
        if ((TIME_now() - last_beat) >= 1000ULL) {
            printf("J8\n");
            trusted_device = false;
            *RSA_key = RSA_keygen();
            rnd_message = 0LL;
        }
    }
#endif
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
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tWaiting for message\n");
#endif
    while (!order->message_received);
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tHS - Message received: %s\n", order->order_buffer);
#endif
    order->message_received = false;
    GCODE_ret_t ret = GCODE_process_command(order);
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tReceived order: I%d\n", (ret.code / 100));
#endif
    switch (ret.code) {
            // I1
        case 100:
        {
            // Initialize the seed every time this function is called
            RAND_init_seed();
            if (RSA_key == NULL) {
                rsa_t key = RSA_keygen();
                RSA_key = &key;
            }
            printf("I2 %lld\n", RSA_key->n);
            printf("I3 %lld\n", RSA_key->e);
            rnd_message = RAND(10007LL, 104729LL);
            int_fast64_t signed_message = RSA_sign(rnd_message, RSA_key);
            printf("I4 %lld\n", signed_message);
            break;
        }
            // I5 with encrypted msg
        case 500:
        {
            int_fast64_t encrypted_msg = atoll((char *) ret.gcode_ret_val);
#ifdef DEBUG_ENABLED
            printf("[DEBUG]\tRec. msg: %lld\n", encrypted_msg);
#endif
            int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
#ifdef DEBUG_ENABLED
            printf("[DEBUG]\tDecrypted msg: %lld\n", msg);
#endif
            if (msg == rnd_message) {
                trusted_device = true;
                last_beat = TIME_now();
                printf("I5\n");
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
