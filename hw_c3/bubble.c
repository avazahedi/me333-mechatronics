#include <stdio.h>
#include <string.h>
#define MAXLENGTH 100       // max length of string input

void getString(char *str);  // helper prototypes
void printResult(char *str);
int greaterThan(char ch1, char ch2);
void swap(char *str, int index1, int index2);

int main(void) {
    int len;                // length of the entered string
    char str[MAXLENGTH];    // input should be no longer than MAXLENGTH

    getString(str);
    len = strlen(str);      // get length of the string, from string.h

    // nested loops to put the string in sorted order
    for (int i = 0; i < (len-1); i++) {
        for (int j = 0; j < (len-1-i); j++) {
            if (greaterThan(str[j], str[j+1]) == 1) {
                swap(str, j, j+1);      // if the two letters are out of order, swap
            }
        }
    }
    printResult(str);       // print the sorted string
    return(0);
}

// HELPER FUNCTIONS //

// Collect string to be sorted from user input.
void getString(char *str) {
    printf("Enter the string you would like to sort (no spaces): \n");
    scanf("%s", str);
}

// Print the resulting string.
void printResult(char *str) {
    printf("Here is the sorted string: ");
    printf("%s\n", str);
}

// Check if one character is greater than another according to ASCII.
// Returns 1 if ch1 is greater than ch2, otherwise returns 0.
int greaterThan(char ch1, char ch2) {
    if ((int) ch1 > (int) ch2) {
        return 1;
    }
    else {
        return 0;
    }
}

// Inside string str, swap the characters at index1 and index2.
void swap(char *str, int index1, int index2) {
    char tmp = str[index1];
    str[index1] = str[index2];
    str[index2] = tmp;
}