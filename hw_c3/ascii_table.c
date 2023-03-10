#include <stdio.h>

int main(void) {
    printf("ASCII table for values 33 to 127\n");
    printf("Number------------Character\n");
    for (int i = 33; i <= 127; i++) {
        printf(" %d   ------------    %c   \n",i,i);
    }
}