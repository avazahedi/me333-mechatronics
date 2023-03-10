// this header file essentially does 2 things:
// defines a constant MY_PI as an approximation of pi
    // now this is available to any C file that includes rad2volume.h
// we've also provided a function prototype called radius2Volume 

// this is called an include guard
// this is a preprocessor command that say if there's never 
// been defined a constant called RAD2VOLUME_H then
// go ahead and define it

// however, if it has been defined, skip everything between the ifndef and endif
    // this is because when we compile this code, we want to make sure
    // that we don't accidentally include rad2volume.h twice during 
    // the same compilation to object code
#ifndef RAD2VOLUME_H
#define RAD2VOLUME_H

#define MY_PI 3.1415926
double radius2Volume(double r);         // function prototype that returns a double and takes in a double

#endif      // end of if statement