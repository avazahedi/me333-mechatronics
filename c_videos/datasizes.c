#include <stdio.h>

int main(void) {
    char a, *bp; short c; int d; long e;       // char a and pointer bp of type char, short int c, int d, long int e
    float f; double g; long double h, *ip;      // floating point f, double precision floating point g, long double precision f.p. h, pointer of type ldpfp

    printf("Size of char:                   %2ld bytes\n",sizeof(a));   // %2ld means 2 spaces w/ type long int
    printf("Size of char pointer:           %2ld bytes\n",sizeof(bp));
    printf("Size of short int:              %2ld bytes\n",sizeof(c));
    printf("Size of int:                    %2ld bytes\n",sizeof(d));
    printf("Size of long int:               %2ld bytes\n",sizeof(e));
    printf("Size of float:                  %2ld bytes\n",sizeof(f));
    printf("Size of double:                 %2ld bytes\n",sizeof(g));
    printf("Size of long double:            %2ld bytes\n",sizeof(h));
    printf("Size of long double pointer:    %2ld bytes\n",sizeof(ip));
    return(0);
}