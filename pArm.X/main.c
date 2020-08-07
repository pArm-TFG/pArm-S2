/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */


volatile char receivedValue = 0;

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

int main(void) {
    system_initialize();
    /*initBoard();
    initUART();
    initPWM();

    TMR1_Initialize();
    TMR2_Initialize();
    initDigitalPorts();*/


    /*while (1) {
        if (receivedValue) {
            printf("%c\n", receivedValue);
            receivedValue = 0;
        }
    }
 

    while (1) {

        if (_ICNFLAG == 1) {
            printf('PIN INTERRUPTION');
            _ICNFLAG = 0;
        }
    }*/


    /*while (1) {      
        if (U1STAbits.FERR == 1) {
            continue;
        }
        if (U1STAbits.OERR == 1) {
            U1STAbits.OERR = 0;
            continue;
        }
        if (U1STAbits.URXDA == 1) {
            printf("%c", U1RXREG);
//            ReceivedChar = U1RXREG;
        }
    }*/

    time_t next = now();
    uint16_t count = 0U;
    while (1) {
        //        printf("%lu\n", now());
        //        printf("main\n");
        //        __delay_ms(150);
        if (now() >= next) {
            /*printf("!\n");
            next += 1000ULL;
            ++count;
            if (count >= next_c) {
                printf("uS: %llu\n", now_us());
                next_c += 20;
            }*/
            printf("Time: %us\n", count);
            next = now() + 1000ULL;
            count += 1U;
        }
        /*if (receivedValue != 0) {
            printf("%c", receivedValue);
        }*/
    }
    /*
    
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
        
        if(PORTBbits.RB0 == 1)
        {
            PORTBbits.RB6 = 1;
        }
        else
        {
            PORTBbits.RB6 = 0;
        }
        
        if(PORTAbits.RA1 == 1)
        {
            PORTBbits.RB5 = 1;
        }
        else
        {
            PORTBbits.RB5 = 0;
        }
        
        if(PORTAbits.RA0 == 1)
        {
            PORTBbits.RB5 = 1;
            PORTBbits.RB6 = 1;
            PORTBbits.RB7 = 1;
        }
        else
        {
            PORTBbits.RB5 = 0;
        }
    }*/

    //    U1TXREG = 'a';
    //    printf("UART\n");
    __delay_ms(1000);
    Servo motor_4 = {&SDC1};
    Servo motor_3 = {&SDC2};
    Servo motor_2 = {&SDC3};
    Servo motor_1 = {&PDC1};
    while (1) {
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
    }
    return 0;
}
