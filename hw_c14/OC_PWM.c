#include "nu32dip.h"          // constants, functions for startup and UART

int main(void) {
  NU32DIP_Startup();          // cache on, interrupts on, LED/button init, UART init

  RPA0Rbits.RPA0R = 0b0101; // enables OC1 on RA0 (pin 2)
  T3CONbits.TCKPS = 0;     // Timer3 prescaler N=1
  PR3 = 2399;              // period = (PR3+1) * N * 20.833 ns = 50000ns, 20 kHz
  TMR3 = 0;                // initial TMR3 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 1800;             // duty cycle = OC1RS/(PR3+1) = 75%
  OC1R = 1800;              // initialize before turning OC1 on; afterward it is read-only
  T3CONbits.ON = 1;        // turn on Timer3
  OC1CONbits.OCTSEL = 1;    // Use Timer 3
  OC1CONbits.ON = 1;       // turn on OC1

  while(1) {
    ;                      // infinite loop
  }
  return 0;
}

