#include "nu32dip.h"
#include <stdio.h>

#define NUMSAMPS 1000       // number of points in waveform
#define PLOTPTS 200         // number of data points to plot
#define DECIMATION 10       // plot every 10th point

static volatile int Waveform[NUMSAMPS];     // waveform
static volatile int ADCarray[PLOTPTS];      // measured values to plot
static volatile int REFarray[PLOTPTS];      // reference values to plot
static volatile int StoringData = 0;        // if this flag = 1, currently storing plot data
static volatile float Kp=0, Ki=0;           // control gains
static volatile int eint;

void makeWaveform();
unsigned int adc_sample_convert(int pin);

void makeWaveform()
{
    // int i = 0, center = 1200, A = 1199;     // square wave
    int i = 0, center = 500, A = 300;     // square wave
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

unsigned int adc_sample_convert(int pin) { // sample & convert the value on the given 
                                           // adc pin the pin should be configured as an 
                                           // analog input in AD1PCFG
    unsigned int elapsed = 0, finish_time = 0;
    AD1CHSbits.CH0SA = pin;                // connect chosen pin to MUXA for sampling
    AD1CON1bits.SAMP = 1;                  // start sampling
    elapsed = _CP0_GET_COUNT();
    finish_time = elapsed + 10;     // increasing # could help reduce noise
    while (_CP0_GET_COUNT() < finish_time) { 
      ;                                   // sample for more than 250 ns
    }
    AD1CON1bits.SAMP = 0;                 // stop sampling and start converting
    while (!AD1CON1bits.DONE) {
      ;                                   // wait for the conversion process to finish
    }
    return ADC1BUF0;                      // read the buffer with the result
}

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void)
{
    static int counter = 0;     // initialize counter once
    static int plotind = 0;     // index for data arrays; counts up to PLOTPTS
    static int decctr = 0;      // counts to store data one every DECIMATION
    // static int adcval = 0;

    // static int eint = 0;
    unsigned int adcval = adc_sample_convert(1);
    int error = Waveform[counter] - adcval;
    eint = eint + error;
    float u = Kp*error + Ki*eint;
    float unew = u + 50.0;
    if (unew > 100.0) {
        unew = 100.0;
    }
    else if (unew < 0.0) {
        unew = 0.0;
    }

    // OC1RS = Waveform[counter];
    OC1RS = (unsigned int) ((unew/100.0) * 2399);

    if (StoringData) {
        decctr++;
        if (decctr == DECIMATION) {         // after DECIMATION control loops,
            decctr = 0;                     // reset decimation counter
            ADCarray[plotind] = adcval;     // store data in global arrays
            REFarray[plotind] = Waveform[counter];
            plotind++;                      // increment plot data index
        }
        if (plotind == PLOTPTS) {           // if max number of plot points is reached,
            plotind = 0;                    // reset the plot index
            StoringData = 0;                // tell main that data is ready to be sent to Python
        }
    }

    counter++;
    if (counter == NUMSAMPS)
    {
        counter = 0;        // roll counter over when end of waveform reached
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

  T3CONbits.TCKPS = 0;     // Timer3 prescaler N=1
  PR3 = 2399;              // period = (PR3+1) * N * 20.833 ns = 50000ns, 20 kHz
  TMR3 = 0;                // initial TMR3 count is 0
  T3CONbits.ON = 1;        // turn on Timer3

  RPA0Rbits.RPA0R = 0b0101; // enables OC1 on RA0 (pin 2)
  T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1
  PR2 = 47999;              // period = (PR2+1) * N * 20.833 ns = 1000000ns, 1 kHz
  TMR2 = 0;                // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0;             // duty cycle = OC1RS/(PR2+1) = 75%
  OC1R = 0;              // initialize before turning OC1 on; afterward it is read-only
  T2CONbits.ON = 1;        // turn on Timer2
  OC1CONbits.OCTSEL = 1;    // Use Timer
  OC1CONbits.ON = 1;       // turn on OC1

  ANSELAbits.ANSA1 = 0;                   // use A1 as AN1
  AD1CON3bits.ADCS = 1;                   // ADC clock period is Tad = 2*(ADCS+1)*Tpb =
                                          //                           2*3*12.5ns = 75ns < 83ns
  AD1CON1bits.ADON = 1;                   // turn on A/D converter

  char message[100];            // message to and from Python
  float kptemp = 0, kitemp = 0; // temporary local gains
  int i = 0;                    // plot data loop counter

  makeWaveform();

  while(1) {
    NU32DIP_ReadUART1(message, sizeof(message)); // wait for a message from MATLAB
    sscanf(message, "%f %f" , &kptemp, &kitemp);
    eint = 0;

    __builtin_disable_interrupts(); // keep ISR disabled as briefly as possible
    Kp = kptemp; // copy local variables to globals used by ISR
    Ki = kitemp;
    __builtin_enable_interrupts(); // only 2 simple C commands while ISRs disabled
    
    StoringData = 1; // message to ISR to start storing data

    while (StoringData) { // wait until ISR says data storing is done
        ; // do nothing 
    }
    
    for (i=0; i<PLOTPTS; i++) { // send plot data to MATLAB
        // when first number sent = 1, MATLAB knows we’re done
        sprintf(message, "%d %d %d\r\n", PLOTPTS-i, ADCarray[i], REFarray[i]);
        NU32DIP_WriteUART1(message);
    }
  }
  return 0;
}
