//Name: Joel Miller
//Username: jmil53@clemson.edu
//Date: 06/25/2023
//Professor: Dr. Drachova
//Course: CPSC 3220



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "isodd.c"
#include "isprime.c"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of command line arguments.\n");
        printf("Usage: %s <number>\n", argv[0]);
        exit(1);
    }

    // Convert command line argument to integer
    int numVal = atoi(argv[1]);
    int cPid;

    pthread_t primeVal1, oddVal2;

    void *primeResult, *oddResult;
   

    // Create a child process
    cPid = fork();

    if (cPid < 0) {
        perror("fork failed");
        exit(1);
    } else if (cPid == 0) {
       
        //file location to the square root function argument 1
        execl("./sqrrt", "sqrrt", argv[1], NULL);

        perror("exec failed");
        exit(1);
    } else {
        // Wait for the child process to complete
        wait(NULL);
      // Create a thread to check if the number is prime
        pthread_create(&primeVal1, NULL, isprime, &numVal);

        pthread_create(&oddVal2, NULL, isodd, &numVal);
        // Wait for the prime thread to complete

        
        pthread_join(primeVal1, &primeResult);
       
        // Create a thread to check if the number is odd
        
        // Wait for the odd thread to complete
        pthread_join(oddVal2, &oddResult);
        
        //New Line
        printf("\n");
       
        // Print the results
        printf("Parent: pid %d, CLA: %d, sqrrt is %.0f, %s, %s\n", getpid(), numVal, sqrt(numVal), (char *)primeResult, (char *)oddResult);
    }

    return 0;
}
