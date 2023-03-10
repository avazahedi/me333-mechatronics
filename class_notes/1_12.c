

typedef struct {
    char fname[100];
    char lname[100];
    int stuid;
    int stuidnum;
} Student;

Student init(Student s){
    int i = 0;
    for (i=0;i<NAMELEN;i++){
        s.fname[i] = '\0';
    }
    sprintf(s.lname, "marchuk");   // put an array into memory
    return s;
}

void initp(Student * sp){
    int i = 0;
    for (i=0;i<NAMELEN;i++){
        sp->fname[i] = '\0';
    }
    sprintf(sp->lname, "marchuk");
}

////////////////////////////////////////////////////////////

int i = 10;
if (i == 10) {
    printf("...");
}

float t = 4.00001;      // sometimes floating point numbers cannot represent a number exactly
                        // because floats don't have enough bits to represent that many values
double t = 4.00001;     // if you're worried about this use a double - but it takes 2x as much memory
if (t >= 4.00001) {
    printf("...");
}

//////////////////////////////////////////////////////////////

int i = 10, j = 0;
if (i==10 & j==9) {
    printf("...");
}

///////////////////////////////////////////////////////////
a loop will be on the quiz
when you call gcc what are the intermediate steps?

/////////////////////////////////////////////////////////
C has a function in the standard library stdlib.h for dynamic memory allocation
- malloc
memory leaks
////////////////////////////////////////////////////////////
