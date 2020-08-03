/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */


#include "utils/defs.h"
#include "pragmas.h"
#include <xc.h>
#ifdef USE_CUSTOM_PRINTF
#include "printf/printf.h"
#else
#include "utils/uart.h"
#endif
#include "init.h"
#include "motor/servo.h"
#include "interrupts.h"
#include "utils/time.h"

//void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
//    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
//}

int main(void) {
    initBoard();
    initUART();
    initPWM();

    TMR1_Initialize();
    TMR2_Initialize();
    initDigitalPorts();
    
    time_t next = now();
    uint16_t count = 0U;
    uint16_t next_c = count + 20U;
    while (1) {
//        printf("%lu\n", now());
//        printf("main\n");
//        __delay_ms(150);
        if (now() >= next) {
            printf("!\n");
            next += 1000ULL;
            ++count;
            if (count >= next_c) {
                printf("uS: %llu\n", now_us());
                next_c += 20;
            }
//            printf("Time: %ld\n", count);
//            next = now() + 1000ULL;
//            ++count;
//            __delay_ms(1000);
        }
    }
    
    while(1)
    {
        if(PORTBbits.RB1 == 1)
        {
            PORTBbits.RB7 = 1;
        }
        else
        {
            PORTBbits.RB7 = 0;
        }
    }
    
//    U1TXREG = 'a';
//    printf("UART\n");
    /*while (1) {
        printf("Hello world!\n\r");
        __delay_ms(1000);
    }
    __delay_ms(1000);
    Servo motor_4 = {&SDC1};
    Servo motor_3 = {&SDC2};
    Servo motor_2 = {&SDC3};
    Servo motor_1 = {&PDC1};
    while (1) {
        /*SDC1 = 4208;
        PDC2 = 4280;
        SDC2 = 4208;
        SDC3 = 4208;
        SDC4 = 4208;
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
       writeAngle(&motor_1, 90);
    }*/
    return 0;
}
