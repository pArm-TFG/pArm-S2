/*
 * File:   main.c
 * Author: javinator9889
 *
 * Created on 11 de junio de 2020, 11:16
 */


#include "pragmas.h"
#include <xc.h>
#include "utils.h"
#include "init.h"


int main(void) {
    initBoard();
    initPWM();
    return 0;
}
