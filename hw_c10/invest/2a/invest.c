#include "nu32dip.h"
#include <xc.h>

#define MAX_YEARS 100

typedef struct {
    double inv0;
    double growth;
    int years;
    double invarray[MAX_YEARS+1];
}   Investment;

int getUserInput(Investment *invp);
void calculateGrowth(Investment *invp);
void sendOutput(double *arr, int years);

int main(void) {
    
    Investment inv;

    while(getUserInput(&inv)) {
        inv.invarray[0] = inv.inv0;
        calculateGrowth(&inv);
        sendOutput(inv.invarray, inv.years);
    }
    return(0);
}

void calculateGrowth(Investment *invp) {

    int i;

    for (i=1; i <= invp->years; i=i+1) {
        invp->invarray[i] = invp->growth * invp->invarray[i-1];
    }
}

int getUserInput(Investment *invp) {

    int valid;

    char msg[100];
    char msg2[100] = {};
    char msg3[100];
    sprintf(msg, "Enter investment, growth rate, number of yrs (up to %d): ",MAX_YEARS); // modified
    NU32DIP_WriteUART1(msg);
    NU32DIP_ReadUART1(msg2, 100);
    sscanf(msg2, "%lf %lf %d\r\n", &(invp->inv0), &(invp->growth), &(invp->years));       // modified
    valid = (invp->inv0 > 0) && (invp->growth > 0) && (invp->years > 0) && (invp->years <= MAX_YEARS);
    sprintf(msg3, "Valid input? %d\r\n",valid);     // modified
    NU32DIP_WriteUART1(msg3);
    if (!valid) {
        NU32DIP_WriteUART1("Invalid input; exiting.\r\n");
    }
    return(valid);
}

void sendOutput(double *arr, int yrs) {

    int i;
    char outstring[100];

    NU32DIP_WriteUART1("\nRESULTS:\r\n\n");  // modified
    for (i=0; i<=yrs; i++) {
        sprintf(outstring,"Year %3d: %10.2f\r\n",i,arr[i]);   // modified
        // NU32DIP_WriteUART1("%s",outstring);    // modified
        NU32DIP_WriteUART1(outstring);    // modified
    }
    NU32DIP_WriteUART1("\n");
}