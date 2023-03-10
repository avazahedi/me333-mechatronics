#include "nu32dip.h"          // constants, funcs for startup and UART

void __ISR(_EXTERNAL_0_VECTOR, IPL2SOFT) Ext0ISR(void) { // step 1: the ISR

    // put a delay for 10ms and then check if the button is pushed
    // this way we can see if the "trigger" was an actual button push
    // or just a mistake
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT() < 400000) { ; } // delay for 400,000 core ticks, 0.01 s (10ms)
    if (PORTBbits.RB7 == 0)          // if button is pressed (pressed: LOW = 0, unpressed: HIGH = 1)
    {
        NU32DIP_YELLOW = 0;                  // LED1 and LED2 on
        NU32DIP_GREEN = 0;
        _CP0_SET_COUNT(0);

        while(_CP0_GET_COUNT() < 10000000) { ; } // delay for 10 M core ticks, 0.25 s

        NU32DIP_YELLOW = 1;                  // LED1 and LED2 off
        NU32DIP_GREEN = 1;
    }
    IFS0bits.INT0IF = 0;            // clear interrupt flag IFS0<3>
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

  while(1) {
      ; // do nothing, loop forever
  }

  return 0;
}
