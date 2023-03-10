#include <stdio.h>

int main(void) {
    char i = 100, j = 240, sum;                 // calculate the sum of char i and char j
    unsigned char iu = 100, ju = 240, sumu;
    signed char is = 100, js = 240, sums;

    sum = i+j; sumu = iu+ju; sums = is+js;      // define sum as i+j
    printf("char            %d + %d = %3d or ASCII %c\n",i,j,sum,sum);  // print i + j = sum or sum converted to ASCII
    printf("unsigned char:  %d + %d = %3d or ASCII %c\n",iu,ju,sumu,sumu);
    printf("signed char:    %d + %d = %3d or ASCII %c\n",is,js,sums,sums);
    return(0);
}

// unsigned char will show 100 + 240 = 84
// this is because 340 is outside of the range of 0 to 255
// in hexadecimal this is 154 -- the 1 gets cut off, so we're left with 54
// 54 in hexadecimal is 84 in decimal