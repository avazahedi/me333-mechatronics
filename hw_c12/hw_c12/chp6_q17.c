#include "nu32dip.h"          // constants, funcs for startup and UART
#define MAX_MESSAGE_LENGTH 200

volatile int flag = 0;

void __ISR(_EXTERNAL_0_VECTOR, IPL2SOFT) Ext0ISR(void) { // step 1: the ISR

    // put a delay for 10ms and then check if the button is pushed
    // this way we can see if the "trigger" was an actual button push
    // or just a mistake

    char message[MAX_MESSAGE_LENGTH];
    long int t = _CP0_GET_COUNT();
    while(_CP0_GET_COUNT() < t+4000000) { ; } // delay for 400,000 core ticks, 0.01 s (10ms)
    if (LATBbits.LATB7 == 0)   // if button is pressed
    {
      if (flag == 0)    // if first time pushed
      {
        _CP0_SET_COUNT(0);
        flag = 1;
        NU32DIP_WriteUART1("Press the USER button to stop the timer.\r\n");
      }
      else if (flag == 1)
      {
        long int q = _CP0_GET_COUNT();
        sprintf(message, "%6.3f seconds\r\n", q/40000000.0);
        NU32DIP_WriteUART1(message);
        flag = 0;
      } 
    }
    IFS0bits.INT0IF = 0;            // clear interrupt flag
}

int main(void) {
  NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  __builtin_disable_interrupts(); // step 2: disable interrupts
  INTCONbits.INT0EP = 0;          // step 3: INT0 triggers on falling edge
  IPC0bits.INT0IP = 2;            // step 4: interrupt priority 2
  IPC0bits.INT0IS = 1;            // step 4: interrupt priority 1
  IFS0bits.INT0IF = 0;            // step 5: clear the int flag
  IEC0bits.INT0IE = 1;            // step 6: enable INT0 by setting IEC0<3>
  __builtin_enable_interrupts();  // step 7: enable interrupts
                                  // Connect RB7 (USER button) to INT0 (RB0)

  NU32DIP_WriteUART1("Press the USER button to start the timer.\r\n");

  char m1[MAX_MESSAGE_LENGTH];

  while (1){ 
    if (flag == 1)
    // {
    //   sprintf(m1, "%lf\r", _CP0_GET_COUNT()/40000000.0);
    //   NU32DIP_WriteUART1(m1);
    // } 
    ;
  }

  return 0;
}
