// 1. PROGRAM COMMENTS (PURPOSE, HISTORY)
// 2. PREPROCESSOR COMMANDS

#include <stdio.h>

// 3. DATA TYPE DEFINITIONS
// 4. GLOBAL VARIABLES

int Global_Var;     // putting this variable declaration outside of any function makes it globally accessible (accessible anywhere afterwards)
                    // note: global variables break modularity, so be careful when using them

// 5. FUNCTION PROTOTYPES

void sendOutput(double *arr, int years);    // function of return type void
                                            // two arguments: a pointer to type double and an int

// 6. MAIN FUNCTION

int main(void) {    // ...
    return(0);
}

// 7. HELPER FUNCTIONS

void sendOutput(double *arr, int years) {
    ...
}