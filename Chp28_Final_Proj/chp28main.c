#include "nu32dip.h"          // config bits, constants, funcs for startup and UART
// include other header files here
#include "encoder.h"
#include "utilities.h"

#define BUF_SIZE 200

int main() 
{
  set_mode(IDLE);
  char buffer[BUF_SIZE];
  NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  NU32DIP_GREEN = 1;  // turn off the LEDs
  NU32DIP_YELLOW = 1;        
  __builtin_disable_interrupts();
  // in future, initialize modules or peripherals here
  UART2_Startup();
  __builtin_enable_interrupts();

  while(1)
  {
    NU32DIP_ReadUART1(buffer,BUF_SIZE); // we expect the next character to be a menu command
    NU32DIP_YELLOW = 1;                   // clear the error LED
    switch (buffer[0]) {
      case 'd':                      // dummy command for demonstration purposes
      {
        int n = 0;
        NU32DIP_ReadUART1(buffer,BUF_SIZE);
        sscanf(buffer, "%d", &n);
        sprintf(buffer,"%d\r\n", n + 1); // return the number + 1
        NU32DIP_WriteUART1(buffer);
        break;
      }
      case 'c':   // read encoder count
      {
        int count;
        WriteUART2("a"); // request the encoder count
        while(!get_encoder_flag()){} // wait for the Pico to respond
        set_encoder_flag(0); // clear the flag so you can read again later
        count = get_encoder_count(); // get the encoder value

        sprintf(buffer, "%d\r\n", count);  // return the number + 1
        NU32DIP_WriteUART1(buffer);
        break;

      }
      case 'e':
      {
        WriteUART2("b");  // request the encoder count
        NU32DIP_WriteUART1("reset encoder\r\n");
        break;
      }
      case 'r':
      {
        sprintf(buffer, "%d\r\n", get_mode());
        NU32DIP_WriteUART1(buffer);
        break;
      }
      case 'q':
      {
        // handle q for quit. Later you may want to return to IDLE mode here. 
        set_mode(IDLE);
        break;
      }
      default:
      {
        NU32DIP_YELLOW = 0;  // turn on LED2 to indicate an error
        break;
      }
    }
  }
  return 0;
}