
char c[10];

// c by itself is the pointer to the array
&c[0] === c

c[0] to c[9]

c[11] = 100
// this essentially saying take the address of c and then add 11, 
// then at that memory address put the value to 100
// this is dangerous because you're overwriting the contents at that memory address 
// since c[11] is no longer part of the actual array c

int t[10]
t+2
// c knows that ints take up 4 memory slots, so it knows that when you say
// t+2 you really mean go 8 slots down from t

////////////////////////////////////////////////////////////////////////

// function prototype - just needs to know return type and arg types
void setZero(char*, int); // sending in the array and the length of the array


int main() {
    char c[10];
    // set every value of c to 0
    int i;
    for (i=0; i<10; i++) {
        c[i] = 0;
    }
    // what if you have lots of arrays and you want to initialize all of them? Make a function

    setZero(c,10);

    char t;
    setZero(&t,1);

    return 0;
}

// function: take an array and set all values to zero
void setZero(char * ch, int len) {
    int i;
    for (i=0; i<len; i++) {
        ch[i] = 0;
    }
}

///////////////////////////////////////////////////////////////////////

// Debug mode in VS Code
// you can put a debug point so when we're in debug mode, it'll run the code up to that point
printf("Hello");

///////////////////////////////////////////////////////////////////////

#include <stdio.h>      // standard library

#include "stdio.h"      // local library 

int j = 1000;
printf("j = %d",j);     // % allows for string substitutions
                        // %d means integer
                        // 4 means at least 4 spaces
                        // %d will use however many spaces is necessary

float k = 1.2345;
printf("k = %5.3f\n",k);  // print at least 5 characters, and 3 of them should be after the dot

int p, q;
scanf("%d %d", &p, &q);     // %f = float
                            // %d = int
                            // %s = string (collection of chars) - expects a pointer ** everything else expects values
                            // %c = char

char name[100];     // in C, we always have to give the size on initialization
scanf("%s %d", name, &q);   // can do just name because name represents a memory address
// use character arrays instead of Strings (not a native C type)
// scanf adds a null character to the end to signal to memory that the string has ended
    // A v a \0
printf("Name = %s\n", name)

// printf uses values, scanf uses addresses

/////////////////////////////////////////////////////////////////////////////

int y[10];
y[0] = 123;
// populating y ...
printf("%d",y); // this would print the address of y
printf("%d",y[0],y[1] /*...*/);

// use a for loop
int i;
for (i /*...*/) {
    printf("%d\n",y[i])
}

//////////////////////////////////////////////////////////////////////

// Math!

int i = 2/3;        // this will be 0
float i = 2/3;      // this will also be 0. Compiler sees dividing two ints, does integer math
float i = 2.0/3.0;  // this will give us 0.666...7 (what we want)

// always better to explicitly define floats with .0

// can also typecast
int t = 2;
float i = ((float)t)/3;

/////////////////////////////////////////////////////////////////////////////

typedef struct {
    char fname[100];
    char lname[100];
    int stuid;
} Students

int main () {
    // ...
    Students s1;    // one student
    s1.stuid = 2;
    s1.fname = "Nick";
    s1.lname = "Marchuck";
    // if you have the variable, use dot . to access

    Students s2;    // new student
    Students * s2p;
    s2p = &s2;
    s2p->stuid = 3;
    // if you have the pointer to the variable, use arrow -> to access
}

//////////////////////////////////////////////////////////////////////////////

// when writing a function, you should use a pointer if passing in an array
// other types it's ok to just pass the value directly (unless you have a specific reason not to)
