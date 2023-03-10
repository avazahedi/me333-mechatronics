#include <stdio.h>
#include "rad2volume.h"

int main(void) {
    double radius = 3.0, volume;
    volume = radius2Volume(radius);
    printf("Pi is approximated as %5.3f.\n",MY_PI);
    printf("The volume of the sphere is %8.4lf.\n",volume);
    return 0;
}

// compile using:
// gcc rad2vol_main.c rad2volume.c -o rad2vol_main -lm