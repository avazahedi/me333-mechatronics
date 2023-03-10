#include "nu32dip.h"
#define MAX_MESSAGE_LENGTH 200

int main(void) {
    char message[MAX_MESSAGE_LENGTH];
    NU32DIP_Startup();
    NU32DIP_WriteUART1("Hello1\n\r");
    int i = 0;

    // B6 to input
    // b7 to output
    // b7 init to off
    TRISBbits.TRISB7 = 0;   // specifically B7 should be an output pin
    TRISBbits.TRISB6 = 1;   // specifically B6 should be an input pin
    LATBbits.LATB7 = 0;     // turn B7 pin off

    while (1) {
        // print i
        springf(message, "%d\n\r", i);
        NU32DIP_WriteUART1(message);

        // turn on b7 for 10us
        LATBbits.LATB7 = 1;
        
        // delay
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000*0.00001){} // 10us

        // turn b7 off
        LATBbits.LATB7 = 0;

        // wait for echo to go high
        while(PORTBbits.RB6 == 0){}
        _CP0_SET_COUNT(0);
        while(PORTBbits.RB6 == 1){}
        unsigned int time = _CP0_GET_COUNT();

        float d;
        d = (time/2)/24000000 * 340;

        sprintf(message, "%f\n\r", d);
        NU32DIP_WriteUART1(message);

        // add 1 to i
        i++;
        _CP0_SET_COUNT(0);  // set the core timer (CP0) to 0
        while(_CP0_GET_COUNT()<24000000){}
    }
}