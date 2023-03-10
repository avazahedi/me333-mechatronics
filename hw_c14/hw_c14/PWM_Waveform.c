#include "nu32dip.h"
#include <stdio.h>
#define NUMSAMPS 1000       // number of points in waveform
static volatile int Waveform[NUMSAMPS];     // waveform

void makeWaveform(int PR)
{
    int i = 0, center = (PR+1)/2, A = (PR+1)/2 - 1;     // square wave
    for (i=0; i<NUMSAMPS; ++i)
    {
        if (i < NUMSAMPS/2)
        {
            Waveform[i] = center + A;
        }
        else
        {
            Waveform[i] = center - A;
        }
    }
}

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void)
{
    static int counter = 0;

    OC1RS = Waveform[counter];

    counter++;
    if (counter == NUMSAMPS)
    {
        counter = 0;
    }
    
    IFS0bits.T2IF = 0;
}


int main(void) {
  NU32DIP_Startup();          // cache on, interrupts on, LED/button init, UART init

  __builtin_disable_interrupts();   // disable interrupts at CPU
  IPC2bits.T2IP = 5;                // set priority 5 subpriority 0
  IPC2bits.T2IS = 0;
  IFS0bits.T2IF = 0;                // clear T2 flag status
  IEC0bits.T2IE = 1;              // enable T2 interrupts
  __builtin_enable_interrupts();    // enable interrupts

  RPA0Rbits.RPA0R = 0b0101; // enables OC1 on RA0 (pin 2)
  T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1
  PR2 = 47999;              // period = (PR2+1) * N * 20.833 ns = 1000000ns, 1 kHz
  TMR2 = 0;                // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 1800;             // duty cycle = OC1RS/(PR2+1) = 75%
  OC1R = 1800;              // initialize before turning OC1 on; afterward it is read-only
  T2CONbits.ON = 1;        // turn on Timer2
  OC1CONbits.OCTSEL = 0;    // Use Timer2
  OC1CONbits.ON = 1;       // turn on OC1

  int pr_int = 47999;

  makeWaveform(pr_int);

  while(1) {
    ;                      // infinite loop
  }
  return 0;
}
