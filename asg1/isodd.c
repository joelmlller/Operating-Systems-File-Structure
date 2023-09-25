//Name: Joel Miller
//Username: jmil53@clemson.edu
//Date: 06/25/2023
//Professor: Dr. Drachova
//Course: CPSC 3220

#include <pthread.h>
#include <stdio.h>

void *isodd(void *arg) {
    int numVal = *(int *)arg; // Get the value passed as argument
    pthread_t tid = pthread_self(); // Get the thread ID

    if (numVal % 2 == 0) { // Check if the number is even
        printf("Thread 2: %lu, %d is not an odd number\n", tid, numVal);
        pthread_exit("not an odd number"); // Exit the thread with a message
    } else {
        printf("Thread 2: %lu, %d is an odd number\n", tid, numVal);
        pthread_exit("an odd number"); // Exit the thread with a message
    }
}
