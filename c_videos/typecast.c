#include <stdio.h>

void printRatio(int numer, int denom) {
    double ratio;
    ratio = numer/denom;
    printf("Ratio, %1d/%1d:                             %5.2lf\n",numer,denom,ratio);
    ratio = numer/((double) denom);     // denom is explicitly typecast to a double, numer implicitly changes to a double to be able to do division
    printf("Ratio, %1d/((double) %1d):                  %5.2lf\n",numer,denom,ratio);
    ratio = ((double) numer)/((double) denom);
    printf("Ratio, ((double) %1d)/((double) %1d):       %5.2lf\n",numer,denom,ratio);
}

// execution begins at the main function
int main(void) {
    int num = 5, den = 2;
    printRatio(num,den);
    return(0);
}

// Types of conversions:
// explicit type conversion - ((double) denom)
// implicit type conversion - what happens to numer in line 7
// promotion - take in a smaller integer type and change it to a larger integer type
//      ex. short int to int