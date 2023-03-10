#include "nu32dip.h"
#include "ultra.h"

void initultra() {
    // b6 as output to trig
    // b6 off
    // b7 as input to echo
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB6 = 0;
    LATBbits.LATB6 = 0;
}

unsigned int readultra() {
    LATBbits.LATB6 = 1;
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<24000000*0.00001){}
    LATBbits.LATB6 = 0;

    while(PORTBbits.RB7 == 0){}
    _CP0_SET_COUNT(0);
    while(PORTBbits.RB7 == 1){}
    return _CP0_GET_COUNT();
}