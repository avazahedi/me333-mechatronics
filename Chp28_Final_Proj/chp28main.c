#include "nu32dip.h"          // config bits, constants, funcs for startup and UART
// include other header files here
#include "encoder.h"
#include "utilities.h"
#include "ina219.h"
#include <stdlib.h>

#define BUF_SIZE 200
#define PR2_ 9599
#define PR3_ 2399
#define PR4_ 14999 // 239999
#define PLOTPTS 100
#define TRACKPTS 1000
#define eint_max 100.0
static volatile int duty_cycle; // %
static volatile float Kp = 0.1;
static volatile float Ki = 0.07;
static volatile int count = 0, count_h = 0, count_traj = 0;
static volatile float eint = 0.0, eint_pos = 0.0, eprev_pos = 0.0;
static volatile float REFarray[PLOTPTS];      // reference values to plot
static volatile int CURarray[PLOTPTS];      // measured values to plot
static volatile float POSarray[PLOTPTS];
static volatile int StoringData = 0;
static volatile float pKp=0.95, pKi=0.0, pKd=0.1;
static volatile float usr_angle=0.0; // deg
static volatile float commanded_current = 5.0;

static volatile float RefTraj[TRACKPTS];    // store trajectory values
static volatile float REFarray2[TRACKPTS];
static volatile float POSarray2[TRACKPTS];

void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Position_Controller(void)
{
  switch(get_mode()) {
    case 3: { // HOLD
      // static float eprev_pos = 0.0;
      // static float eint_pos = 0.0;

      WriteUART2("a"); // request the encoder count
      while(!get_encoder_flag()){} // wait for the Pico to respond
      set_encoder_flag(0); // clear the flag so you can read again later
      int p = get_encoder_count(); // get the encoder value

      float enc_ang = (float) p * 360.0/1400.0;

      int error = usr_angle - enc_ang;
      eint_pos = eint_pos + error;
      float u = pKp*error + pKi*eint_pos + pKd*(error - eprev_pos)/0.005;

      // if (u < -100.0) {
      //   u = -100.0;
      // }
      // else if (u > 100.0) {
      //   u = 100.0;
      // }

      commanded_current = u;

      eprev_pos = error;

      // char msg[100];
      // sprintf(msg, "enc_ang: %f\r\nusr_angle: %f\r\n", enc_ang, usr_angle);
      // NU32DIP_WriteUART1(msg);

      POSarray[count_h] = enc_ang;     // store data in global arrays
      REFarray[count_h] = usr_angle;                // reference value
      count_h++;

      // char msg[100];
      // sprintf(msg, "posarray: %f\r\nrefarray: %f\r\n", POSarray[count_h], REFarray[count_h]);
      // NU32DIP_WriteUART1(msg);

      if (count_h == PLOTPTS) {
        count_h = 0;
        eint_pos = 0;
        eprev_pos = 0;
        set_mode(IDLE);
      }
      break;
    }

    case 4: { // TRACK
      // static float eprev_pos = 0.0;
      // static float eint_pos = 0.0;

      WriteUART2("a"); // request the encoder count
      while(!get_encoder_flag()){} // wait for the Pico to respond
      set_encoder_flag(0); // clear the flag so you can read again later
      int p = get_encoder_count(); // get the encoder value

      float ang = (float) p * 360.0/1400.0;

      float ref_traj = RefTraj[count_traj];

      int error = ref_traj - ang;
      eint_pos = eint_pos + error;
      float u = pKp*error + pKi*eint_pos + pKd*(error - eprev_pos)/0.005;

      commanded_current = u;

      eprev_pos = error;

      POSarray2[count_traj] = ang;     // store data in global arrays
      REFarray2[count_traj] = RefTraj[count_traj];                // reference value
      count_traj++;

      if (count_traj == TRACKPTS) {
        count_traj = 0;
        // eint_pos = 0;
        // set_mode(HOLD);
      }

      break;
    }
  }

  IFS0bits.T4IF = 0;          // clear the interrupt flag
}

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Current_Controller(void)
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
        // static float eint = 0.0;

        unsigned int val = 0;
        if (count < 25) {val = 200;}  // mA
        else if (count < 50) {val = -200;}
        else if (count < 75) {val = 200;}
        else {val = -200;}

        int current_sensor = INA219_read_current();
        int error = -1*(val - current_sensor);
        eint = eint + error;
        float u = Kp*error + Ki*eint;
        
        if (u > 100.0) {
          u = 100.0;
        }
        else if (u < -100.0) {
            u = -100.0;
        }

        if (u < 0)
        {
          OC1RS = (int) ((u/-100.0) * PR3_);
          LATBbits.LATB2 = 1;
        }
        else
        {
          OC1RS = (int) ((u/100.0) * PR3_);
          LATBbits.LATB2 = 0;
        }

        if (StoringData) {
          CURarray[count] = current_sensor;     // store data in global arrays
          REFarray[count] = val;                // reference value
          count++;
        }

        if (count == PLOTPTS) {
          count = 0;
          eint = 0;
          StoringData = 0; // reset the flag
          set_mode(IDLE);
        }

        break;
      }
      case 3: {  // HOLD
        // static float eint = 0.0;

        int current_sensor = INA219_read_current();
        float error = -1*(commanded_current - current_sensor);
        eint = eint + error;
        float u = Kp*error + Ki*eint;
        
        if (u > 100.0) {
          u = 100.0;
        }
        else if (u < -100.0) {
            u = -100.0;
        }

        // char msg[100];
        // sprintf(msg, "commanded current: %f\r\ncurrent sensor: %f\r\n", commanded_current, current_sensor);
        // NU32DIP_WriteUART1(msg);

        if (u < 0)
        {
          OC1RS = (int) ((u/-100.0) * PR3_);
          LATBbits.LATB2 = 1;
        }
        else
        {
          OC1RS = (int) ((u/100.0) * PR3_);
          LATBbits.LATB2 = 0;
        }
        
        break;
      }
      case 4: {  // TRACK
        // NU32DIP_WriteUART1("inside t4 isr\n");
        // static float eint = 0.0;

        int current_sensor = INA219_read_current();
        float error = -1*(commanded_current - current_sensor);
        eint = eint + error;
        float u = Kp*error + Ki*eint;
        
        if (u > 100.0) {
          u = 100.0;
        }
        else if (u < -100.0) {
            u = -100.0;
        }

        if (u < 0)
        {
          OC1RS = (int) ((u/-100.0) * PR3_);
          LATBbits.LATB2 = 1;
        }
        else
        {
          OC1RS = (int) ((u/100.0) * PR3_);
          LATBbits.LATB2 = 0;
        }

        break;
      }
    }

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

  IPC4bits.T4IP = 5;                // set priority 5 subpriority 0
  IPC4bits.T4IS = 1;
  IFS0bits.T4IF = 0;                // clear T4 flag status
  IEC0bits.T4IE = 1;              // enable T4 interrupts

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
  OC1CONbits.OCTSEL = 1;    // Use Timer2
  OC1CONbits.ON = 1;       // turn on OC1

  // for position control ISR (200 Hz)
  T4CONbits.TCKPS = 4;     // Timer4 prescaler
  PR4 = PR4_;              // 
  TMR4 = 0;                // initial TMR4 count is 0
  T4CONbits.ON = 1;        // turn on Timer4

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
      case 'g':
      {
        float kpt = 0, kit = 0;
        NU32DIP_ReadUART1(buffer,BUF_SIZE);
        sscanf(buffer, "%f %f", &kpt, &kit);
        Kp = kpt;
        Ki = kit;
        break;
      }
      case 'h':
      {
        sprintf(buffer, "%f\r\n", Kp);
        NU32DIP_WriteUART1(buffer);

        sprintf(buffer, "%f\r\n", Ki);
        NU32DIP_WriteUART1(buffer);
        break;
      }
      case 'i':
      {
        float kpt = 0, kit = 0, kdt = 0;
        NU32DIP_ReadUART1(buffer,BUF_SIZE);
        sscanf(buffer, "%f %f %f", &kpt, &kit, &kdt);
        pKp = kpt;
        pKi = kit;
        pKd = kdt;
        break;
      }
      case 'j':
      {
        sprintf(buffer, "%f\r\n", pKp);
        NU32DIP_WriteUART1(buffer);

        sprintf(buffer, "%f\r\n", pKi);
        NU32DIP_WriteUART1(buffer);

        sprintf(buffer, "%f\r\n", pKd);
        NU32DIP_WriteUART1(buffer);
        break;
      }
      case 'k':
      {
        StoringData = 1;
        set_mode(ITEST);
        while (StoringData) {;} // while storing data, do nothing

        sprintf(buffer, "%d\r\n", PLOTPTS); // send number of samples
        NU32DIP_WriteUART1(buffer);

        for (int i=0; i<PLOTPTS; i++) { // send plot data to MATLAB
          // when first number sent = 1, MATLAB knows we’re done
          sprintf(buffer, "%d %d \r\n", CURarray[i], REFarray[i]);
          NU32DIP_WriteUART1(buffer);
        }

        break;
      }
      case 'l': // go to angle
      {
        WriteUART2("b");
        NU32DIP_ReadUART1(buffer,BUF_SIZE);
        sscanf(buffer, "%f", &usr_angle);

        sprintf(buffer, "%d\r\n", PLOTPTS); // send number of samples
        NU32DIP_WriteUART1(buffer);

        set_mode(HOLD);

        while (get_mode() == HOLD) {;}

        for (int i=0; i<PLOTPTS; i++) { // send plot data to MATLAB
          // when first number sent = 1, MATLAB knows we’re done
          sprintf(buffer, "%f %f \r\n", POSarray[i], REFarray[i]);
          NU32DIP_WriteUART1(buffer);
        }
        break;
      }
      case 'm': // Step Trajectory
      {
        for (int i=0; i < TRACKPTS; i++)
        {
          float refpos = 0.0;
          NU32DIP_ReadUART1(buffer,BUF_SIZE);
          sscanf(buffer, "%f\n", &refpos);
          // StepTraj[i] = refpos;
          RefTraj[i] = refpos;
        }

        break;
      }
      case 'n': // Cubic Trajectory
      {
        for (int i=0; i < TRACKPTS; i++)
        {
          float refpos = 0.0;
          NU32DIP_ReadUART1(buffer,BUF_SIZE);
          sscanf(buffer, "%f\n", &refpos);
          // StepTraj[i] = refpos;
          RefTraj[i] = refpos;
        }
        break;
      }
      case 'o':
      {
        sprintf(buffer, "%d\r\n", TRACKPTS); // send number of samples
        NU32DIP_WriteUART1(buffer);

        set_mode(TRACK);

        for (int i=0; i<TRACKPTS; i++) { // send plot data to MATLAB
          sprintf(buffer, "%f %f \r\n", POSarray2[i], REFarray2[i]);
          NU32DIP_WriteUART1(buffer);
        }
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