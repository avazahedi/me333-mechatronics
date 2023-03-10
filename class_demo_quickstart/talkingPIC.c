#include "nu32dip.h"          // constants, funcs for startup and UART

#define MAX_MESSAGE_LENGTH 200

int main(void) {
  char message[MAX_MESSAGE_LENGTH];
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  while (1) {
    NU32DIP_ReadUART1(message, MAX_MESSAGE_LENGTH);  // get message from computer
    NU32DIP_WriteUART1(message);                     // send message back
    NU32DIP_WriteUART1("\r\n");                      // carriage return and newline
    NU32DIP_GREEN = !NU32DIP_GREEN;                       // toggle the LEDs
    NU32DIP_YELLOW = !NU32DIP_YELLOW;
  }
  return 0;
}
