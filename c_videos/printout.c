#include <stdio.h>  // gives us access to print function

int main(void) {    // main returns an int and takes no arguments (void)
    int i; float f; double d; char c;

    i = 32; f = 4.278; d = 4.278; c = 'k';  // 'k' means we are going take the character k and convert it to a numerical value (happens to be 107 in ASCII)
    printf("Formatted output:\n");          // printf prints to the screen or the standard output - everything inside is in double quotes ""
    printf(" i = %4d c = '%c'\n",i,c);      // %4d means print an integer with 4 spaces, %c means print out a character
    printf(" f = %19.17f\n",f);             // %19.17 means print float with 19 spaces, 17 of which will come after the decimal point
    printf(" d = %19.17lf\n",d);            // same thing, lf is long float
    return(0);      // returning 0 indicates that the program completed successfully
}