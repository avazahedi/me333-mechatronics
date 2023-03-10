#ifndef UTILITIES__H__
#define UTILITIES__H__

#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro

#include "nu32dip.h"

// an enum of possible states, public in the utilities h file
enum mode_t {IDLE, PWM, ITEST, HOLD, TRACK}; 

// public function prototypes in the utilities h file
// function content in the utilities c file:
volatile enum mode_t get_mode(void);
void set_mode(enum mode_t s);

#endif // ENCODER__H__
