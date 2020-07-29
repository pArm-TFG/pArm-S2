/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */


#include "utils/defs.h"
#include "pragmas.h"
#include <xc.h>
#include "utils/utils.h"
#include "init.h"
#include "motor/servo.h"
#include "interrupts.h"
#include "utils/time.h"
#include "utils/uart.h"


int main(void) {
    initBoard();
    initPWM();
    initInterrupts();
    initUART(9600);
//    set_time(0);
    /*while (1) {
        SDC1 = 18720;
        SDC2 = 18720;
        SDC3 = 18720;
        SDC4 = 18720;
        __delay32(1E6);
        SDC1 = 37440;
        SDC2 = 37440;
        SDC3 = 37440;
        SDC4 = 37440;
        __delay32(1E6);
        SDC1 = 0;
        SDC2 = 0;
        SDC3 = 0;
        SDC4 = 0;
    }*/
    /*const int MAX_DUTY_CYCLE = 4208;
    const time_t DUTY_CYCLE_PERIOD = (time_t) 200;
    int i = 0;
    char isAscending = 1;
    while (1) {
        i += isAscending;
        if (i == MAX_DUTY_CYCLE - 1)
            isAscending = -1;
        else if (i == 1)
            isAscending = 1;
        SDC4 = i;
        SDC3 = i;
        SDC2 = i;
        SDC1 = i;
        i += isAscending;
        
        __delay_ms(1000);
//        __delay32(1E4);

//        if (i == 37440) isAscending = -1;
//        else if (i == 0) isAscending = 1;
//        __delay_ms(500);
//        delay_ms(500);
    }*/
    Servo motor_4 = {&SDC1};
    Servo motor_3 = {&SDC2};
    Servo motor_2 = {&SDC3};
    Servo motor_1 = {&PDC1};
//    const char *message = "Hello world!\n";
//    foreach(char, character, message) {
//        U1TXREG = character;
//    }
//    printf("Hello world!\n");
//    const Servo test_pin4 = {0, &PDC4, 0UL};
//    const Servo test_pin1 = {0, &PDC1, 0UL};
//    while (1) {
//        writeAngle(&test_pin4, 90);
//        writeAngle(&test_pin1, 90);
//    }
    while (1) {
//        printf("looping\n");
//        writeAngle(&motor_4, 90);
        //        write(1, 90);
//        SDC1 = 2770;
//        __delay_ms(1000);
//        writeAngle(&motor_4, 180);
        //        write(1, 180);
//        SDC1 = 1402;
//        __delay_ms(1000);
//        writeAngle(&motor_4, 0);
        //        write(1, 0);
        /*SDC1 = 4208;
        PDC2 = 4280;
        SDC2 = 4208;
        SDC3 = 4208;
        SDC4 = 4208;*/
        writeAngle(&motor_4, 90);
        __delay_ms(200);
        writeAngle(&motor_3, 45);
        __delay_ms(200);
        writeAngle(&motor_2, 0);
        __delay_ms(200);
        writeAngle(&motor_1, 180);
        __delay_ms(200);
        writeAngle(&motor_4, 0);
        __delay_ms(200);
        writeAngle(&motor_3, 180);
        __delay_ms(200);
        writeAngle(&motor_2, 60);
        __delay_ms(200);
        writeAngle(&motor_1, 30);
        __delay_ms(200);
        writeAngle(&motor_4, 180);
        __delay_ms(200);
        writeAngle(&motor_3, 90);
        __delay_ms(200);
        writeAngle(&motor_2, 45);
        __delay_ms(200);
        writeAngle(&motor_1, 0);
        __delay_ms(200);
        writeAngle(&motor_4, 0);
        __delay_ms(200);
        writeAngle(&motor_3, 30);
        __delay_ms(200);
        writeAngle(&motor_2, 120);
        __delay_ms(200);
        writeAngle(&motor_1, 90);
        __delay_ms(200);
//        writeAngle(&motor_1, 90);
    }
    return 0;
}
