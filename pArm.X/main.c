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
#include <libpic30.h>
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
#include "utils/buffer.h"

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
volatile order_t *order = NULL;

static double64_t motor_movement_finished_time = LDBL_MAX;
static time_t movement_start_time = UINT64_MAX;
volatile barrier_t *barrier;
static volatile bool is_moving = false;
#ifdef LIMIT_SWITCH_ENABLED
volatile uint_fast8_t limit_switch_map[4] = {0};
#endif
#ifdef CLI_MODE
static bool show_cursor = true;
#else
static time_t last_beat = 0ULL;
static time_t turn_led_off_time = UINT64_MAX;
/**
 * Application's random message used for authoring the remote
 * device. When the host changes this message is destroyed.
 */
static int_fast64_t rnd_message;

/**
 * Flag active when the handshake has been successful.
 * If there is any kind of error during the message exchange
 * the device will be marked as untrusted.
 */
volatile bool trusted_device = false;
#endif

void setup(void);
void loop(void);
char check_motor_status(void);
void handle_order(void);
//void do_movement(double64_t expected_time);
void update_motor_time(motor_t *motor);
#ifndef CLI_MODE
void do_handshake(void);
void beat(int_fast64_t encrypted_msg);
#endif

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
    order = (order_t *) malloc(sizeof (order_t));
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
#ifdef LIMIT_SWITCH_ENABLED
    CN_Init(limit_switch_map);
#endif

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

#ifdef DEBUG_ENABLED
    printf("[SETUP]\tChecking motor status...\n");
#endif
    // Init the planner so the motors are available
#ifdef LIMIT_SWITCH_ENABLED    
    PLANNER_init(barrier, limit_switch_map);
#else
    PLANNER_init(barrier);
#endif

    // Calibrate the motors. If someone returns
    // not OK, stop execution until rebooted
    // and notify turning on an LED
    if (false && check_motor_status() == EXIT_FAILURE) {
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
    // Move the motors to home position
    PLANNER_go_home();

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
#ifdef DEBUG_ENABLED
        printf("[DEBUG]\tDevice not trusted... Waiting I1\n");
#endif
        do_handshake();
    }
    PORTBbits.RB7 = trusted_device;
#else
    if (show_cursor) {
        printf("$> ");
        show_cursor = false;
    }
#endif
    if (order->message_received) {
        order->message_received = false;
        GCODE_ret_t ret = GCODE_process_command(order);
        switch (ret.code) {
                // G0
            case 0:
            {
                if (ret.is_err) {
                    // Coordinates missing for G0
                    printf("J8\n");
                } else {
                    point_t *position = (point_t *) ret.gcode_ret_val;
                    double64_t expected_time = PLANNER_move_xyz(*position);
#ifdef DEBUG_ENABLED
                    printf("[DEBUG]\tMoving motors to x: %Lf, y: %Lf, z: %Lf\n",
                            position->x, position->y, position->z);
#endif
                    free(position);
                    if (expected_time == -1.0F) {
                        // Out-of-range
                        printf("J4\n");
                    } else {
                        is_moving = true;
                        printf("J1 %lf\n", (expected_time / 1000.0F));
                        movement_start_time = TIME_now_us();
                        motor_movement_finished_time =
                                movement_start_time + expected_time;
                    }
                }
                break;
            }
                // G1
            case 1:
            {
                if (ret.is_err) {
                    // Coordinates missing for G1
                    printf("J9\n");
                } else {
                    angle_t *angles = (angle_t *) ret.gcode_ret_val;
                    double64_t expected_time = PLANNER_move_angle(*angles);
#ifdef DEBUG_ENABLED
                    printf("[DEBUG]\tMoving motors to t0: %Lf, t1: %Lf, t2: %Lf\n",
                            angles->theta0, angles->theta1, angles->theta2);
#endif
                    free(angles);
                    if (expected_time == -1.0F) {
                        // Out-of-range
                        printf("J4\n");
                    } else {
                        is_moving = true;
                        printf("J1 %lf\n", (expected_time / 1000.0F));
                        movement_start_time = TIME_now_us();
                        motor_movement_finished_time =
                                movement_start_time + expected_time;
                    }
                }
                break;
            }
                // G28
            case 28:
            {
                double64_t expected_time = PLANNER_go_home();
                is_moving = true;
                printf("J1 %lf\n", (expected_time / 1000.0F));
                movement_start_time = TIME_now_us();
                motor_movement_finished_time =
                        movement_start_time + expected_time;
#ifdef DEBUG_ENABLED
                printf("[DEBUG]\tMoving motors to home position...\n");
#endif
                break;
            }
                // M1
            case 10:
            {
                if (!is_moving)
                    printf("J5\n");
                else {
                    uint8_t ret = PLANNER_stop_moving();
                    if (ret == EXIT_SUCCESS)
                        printf("M1\n");
                    else // Motors not moving
                        printf("J5\n");
                    is_moving = false;
                }
                break;
            }
                // M114
            case 1140:
            {
                point_t *position = PLANNER_get_position();
                printf("G0 X%Lf Y%Lf Z%Lf\n",
                        position->x,
                        position->y,
                        position->z);
                free(position);
                break;
            }
                // M280
            case 2800:
            {
                angle_t *position = PLANNER_get_angles();
                printf("G1 X%Lf Y%Lf Z%Lf\n",
                        position->theta0,
                        position->theta1,
                        position->theta2);
                free(position);
                break;
            }
#ifndef CLI_MODE
                // I6
            case 600:
            {
                char *ord_msg = (char *) ret.gcode_ret_val;
                int_fast64_t encrypted_msg = atoll(ord_msg);
                int_fast64_t dec_msg = RSA_decrypt(encrypted_msg, RSA_key);
                if (dec_msg == rnd_message) {
                    trusted_device = false;
                    *RSA_key = RSA_keygen();
                    rnd_message = 0LL;
                } else {
                    printf("J10\n");
                }
                free(ord_msg);
                break;
            }
                // I7
            case 700:
            {
                char *ord_msg = (char *) ret.gcode_ret_val;
                int_fast64_t encrypted_msg = atoll(ord_msg);
                beat(encrypted_msg);
                free(ord_msg);
                break;
            }
#endif
                // Invalid GCODE found
            default:
            {
                printf("J3\n");
                break;
            }
        }
        BUFFER_free(order->order_buffer);
#ifdef CLI_MODE
        show_cursor = true;
#endif
    }
#ifndef USE_MOTOR_TMRS
    if (is_moving) {
        update_motor_time(motors.base_motor);
        update_motor_time(motors.lower_arm);
        update_motor_time(motors.upper_arm);
        if (TIME_now_us() >= motor_movement_finished_time) {
            printf("J21\n");
            is_moving = false;
        }
#else
    if (is_moving && BARRIER_all_done(barrier)) {
        // Notify all motors have finished their movement
        printf("J21\n");
        // and clear barrier interrupt flag
        BARRIER_clr(barrier);
        is_moving = false;
#endif
#ifdef CLI_MODE
        show_cursor = true;
#endif
    }
#ifndef CLI_MODE
    if (trusted_device) {
        // If last beat happened at least 1 second ago
        // untrust the device and send 'J6' for informing
        if ((TIME_now() - last_beat) >= 1000ULL) {
            printf("J10\n");
            trusted_device = false;
            *RSA_key = RSA_keygen();
            PORTBbits.RB7 = 0;
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
    return MOTOR_calibrate(motors.upper_arm);
}

#ifndef CLI_MODE

inline void do_handshake(void) {
#ifdef DEBUG_ENABLED
    printf("[DEBUG]\tWaiting for handshake message...\n");
#endif
    while (!order->message_received);
    order->message_received = false;
    GCODE_ret_t ret = GCODE_process_command(order);
    switch (ret.code) {
            // I1
        case 100:
        {
            // Initialize the seed every time this function is called
            RAND_init_seed();
            if (RSA_key == NULL) {
#ifdef DEBUG_ENABLED
                printf("[DEBUG]\tGenerating new RSA keys...\n");
#endif
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
            char *ord_msg = (char *) ret.gcode_ret_val;
            int_fast64_t encrypted_msg = atoll(ord_msg);
            int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
            if (msg == rnd_message) {
                trusted_device = true;
                last_beat = TIME_now();
                printf("I5\n");
            } else {
                printf("J6\n");
                trusted_device = false;
            }
            free(ord_msg);
            break;
        }
        default:
        {
            // Untrusted device
            printf("J10\n");
            break;
        }
    }
}
#endif

void update_motor_time(motor_t *motor) {
    if (!motor->movement_finished) {
        motor->current_movement_count =
                (TIME_now_us() - movement_start_time);
        if (motor->current_movement_count >= motor->movement_duration) {
            motor->movement_finished = true;
            motor->angle_us = motor->current_movement_count;
            motor->current_movement_count = 0ULL;
        }
    }
}

#ifndef CLI_MODE

inline void beat(int_fast64_t encrypted_msg) {
    printf("BEAT!\n");
    int_fast64_t msg = RSA_decrypt(encrypted_msg, RSA_key);
    if (msg == rnd_message) {
        last_beat = TIME_now();
        PORTBbits.RB5 = 1;
        PORTBbits.RB6 = 1;
        PORTBbits.RB7 = 1;
        turn_led_off_time = (TIME_now() + 100ULL);
    }
}
#endif
