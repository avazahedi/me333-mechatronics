#include <math.h>           // math header file - gives us access to the power function
#include "rad2volume.h"     // has access to rad2volume header file

// only meant for internal use - that's why it's only in the C code and not in the header file
double cuber(double x) {
    return(pow(x,3.0));
}

// this is the public function that we want to make available to other codes that are going to use this library
// that is why radius2Volume has a function prototype in the header file
double radius2Volume(double rad) {
    return((4.0/3.0)*MY_PI*cuber(rad));
}