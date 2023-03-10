// 1. PROGRAM COMMENTS (PURPOSE, HISTORY)

/*
invest.c

This program takes an initial investment amount, an expected annual return rate, and the number of 
years, and calculates the growth of the investment. The main point of this program, though, is to 
demonstrate some C syntax.

HISTORY:
Dec 20, 2011    Created by Kevin Lynch
Jan 4, 2012     Modified by Kevin Lynch
*/

/*********************************************
2. PREPROCESSOR COMMANDS        ==SecA.4.3==
**********************************************/

#include <stdio.h>      // allows you to print to screen and read from keyboard
#define MAX_YEARS 100   // define a constant
                        // different from a variable, in that a variable can change during the program - a constant cannot

/*********************************************
3. DATA TYPE DEFINITIONS        ==SecA.4.4==
**********************************************/

typedef struct {        // defining a new data type - the type of new type is a structure
    double inv0;
    double growth;
    int years;
    double invarray[MAX_YEARS+1];   // array called invarray with MAX_YEARS+1 elements of type double
}   Investment;         // new data type is called Investment
// Investment carries a few different variables with it so we can pass it around as a single packet

/*********************************************
4. GLOBAL VARIABLES   ==SecA.4.2, SecA.4.5==
**********************************************/

// no global variables in this program

/*********************************************
5. HELPER FUNCTION PROTOTYPES   ==SecA.4.2==
**********************************************/

int getUserInput(Investment *invp);
void calculateGrowth(Investment *invp);
void sendOutput(double *arr, int years);

/*********************************************
6. MAIN FUNCTION                ==SecA.4.2==
**********************************************/

int main(void) {
    
    Investment inv;

    while(getUserInput(&inv)) {
        inv.invarray[0] = inv.inv0;
        calculateGrowth(&inv);
        sendOutput(inv.invarray, inv.years);
    }
    return(0);
}

/*********************************************
7. HELPER FUNCTIONS             ==SecA.4.2==
**********************************************/

void calculateGrowth(Investment *invp) {

    int i;

    for (i=1; i <= invp->years; i=i+1) {        // initialize counter, 
                                                // perform a test in the middle - if it's true, do what's in the loop
                                                    // checking if i is <= the number of years that we want to track the investment
                                                // do the modification i=i+1
                                                // repeat until condition not true
        invp->invarray[i] = invp->growth * invp->invarray[i-1];
    }

    // notation invp->invarray says get the variable inside the structure that's pointed to by the invp pointer
    // note that accessing a structure uses the dot - Investment inv, inv.invarray[0] (see main function)
    // we need to use arrow notation -> because we have the pointer instead

}

int getUserInput(Investment *invp) {

    int valid;      // local variable valid

    printf("Enter investment, growth rate, number of yrs (up to %d): ",MAX_YEARS);
    scanf("%lf %lf %d", &(invp->inv0), &(invp->growth), &(invp->years));    // looking for two doubles and an int
                                                                                // long floats are the same as doubles
                                                                            // scanf needs to know where to put the inputs
                                                                                // need memory addresses of the variables 
                                                                                // we want to store inputs in
    valid = (invp->inv0 > 0) && (invp->growth > 0) && (invp->years > 0) && (invp->years <= MAX_YEARS);  // checks that values are positive 
                                                                                                        // and years <= MAX_YEARS
                                                                                                            // && means and
    printf("Valid input? %d\n",valid);      // 0 = False, 1 = True
    if (!valid) {                           // if not equal to 1, aka if not true
        printf("Invalid input; exiting.\n");
    }
    return(valid);
}

void sendOutput(double *arr, int yrs) {

    int i;
    char outstring[100];        // array of 100 elements of type char

    printf("\nRESULTS:\n\n");
    for (i=0; i<=yrs; i++) {
        sprintf(outstring,"Year %3d: %10.2f\n",i,arr[i]);   // sprintf prints to a string called outrstring
                                                            // outstring can have a max length of 100
                                                            // because we've only allocated 100 chars for it
        printf("%s",outstring);     // now that we've generated the string, we can just print it to the screen
                                    // %s means we're looking for a string
    }
    printf("\n");
}