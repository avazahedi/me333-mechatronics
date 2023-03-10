#include "nu32dip.h"

int main(void) {

    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT() < 4 * 24000000) {
        ;
    }

}