#include "nu32dip.h"
#include "helper.h"
#include <xc.h>

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