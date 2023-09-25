//Name: Joel Miller
//Username: jmil53@clemson.edu
//Date: 06/25/2023
//Professor: Dr. Drachova
//Course: CPSC 3220

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of command line arguments.\n");
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[1]);
    double result = sqrt(num);

    printf("Child: pid %d, ppid %d, sqrrt is %.0f\n", getpid(), getppid(), result);
    printf("\n");
    return 0;
}
