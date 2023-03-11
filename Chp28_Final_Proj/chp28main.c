#include "nu32dip.h"          // config bits, constants, funcs for startup and UART
// include other header files here
#include "encoder.h"
#include "utilities.h"
#include "ina219.h"
#include <stdlib.h>

#define BUF_SIZE 200
#define PR2_ 9599
#define PR3_ 2399
static volatile int duty_cycle; // %

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void)
{
    switch(get_mode()) {
      case 0: {  // IDLE
        LATBbits.LATB2 = 0; // motor direction, 0=+, 1=-
        OC1RS = 0;
        break;
      }
      case 1: {  // PWM
        if (duty_cycle < 0) {
          LATBbits.LATB2 = 1;
          OC1RS = (int) ((float)duty_cycle/-100.0 * (PR3_ + 1));
        }
        else {
          LATBbits.LATB2 = 0;
          OC1RS = (int) ((float)duty_cycle/100.0 * (PR3_ + 1));
        }
        break;
      }
      case 2: {  // ITEST
        break;
      }
      case 3: {  // HOLD
        break;
      }
      case 4: {  // TRACJ
        break;
      }
    }
    // OC1RS = 0.25*(PR2_+1);
    // LATBbits.LATB2 = 1;

    IFS0bits.T2IF = 0;          // clear the interrupt flag
}

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
  INA219_Startup();

  IPC2bits.T2IP = 5;                // set priority 5 subpriority 0
  IPC2bits.T2IS = 0;
  IFS0bits.T2IF = 0;                // clear T2 flag status
  IEC0bits.T2IE = 1;              // enable T2 interrupts

  __builtin_enable_interrupts();

  // for PWM
  TRISBCLR = 0x4;
  T3CONbits.TCKPS = 0;     // Timer3 prescaler N=1
  PR3 = PR3_;              // period = (PR3+1) * N * 20.833 ns = 50000ns, 20 kHz
  TMR3 = 0;                // initial TMR3 count is 0
  T3CONbits.ON = 1;        // turn on Timer3

  // for ISR
  RPA0Rbits.RPA0R = 0b0101; // enables OC1 on RA0 (pin 2)
  T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1
  PR2 = PR2_;              // period = (PR2+1) * N * 20.833 ns = 200000ns, 5 kHz
  TMR2 = 0;                // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0;    // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 0;     // initialize before turning OC1 on; afterward it is read-only
  T2CONbits.ON = 1;        // turn on Timer2
  OC1CONbits.OCTSEL = 1;    // Use Timer
  OC1CONbits.ON = 1;       // turn on OC1

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
      case 'b':
      {
        sprintf(buffer, "%f\r\n", INA219_read_current());
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
      case 'f':
      {
        set_mode(PWM);
        int n = 0;
        NU32DIP_ReadUART1(buffer,BUF_SIZE);
        sscanf(buffer, "%d", &n);
        sprintf(buffer,"%d\r\n", n);
        NU32DIP_WriteUART1(buffer);

        duty_cycle = n;
        break;
      }
      case 'p':
      {
        set_mode(IDLE);
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