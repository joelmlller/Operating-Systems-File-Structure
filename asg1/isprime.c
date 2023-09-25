//Name: Joel Miller
//Username: jmil53@clemson.edu
//Date: 06/25/2023
//Professor: Dr. Drachova
//Course: CPSC 3220


#include <pthread.h>
#include <stdio.h>

void *isprime(void *arg) {
    int numVal = *(int *)arg; // Get the value passed as argument
    pthread_t tid = pthread_self(); // Get the thread ID

    int isPrime = 1;
    for (int i = 2; i <= numVal / 2; i++) { // Check for factors of numVal
        if (numVal % i == 0) {
            isPrime = 0; // If a factor is found, mark the number as not prime
            break; // Exit the loop since we already know it's not prime
        }
    }

    if (isPrime) { // If isPrime is still true, the number is prime
        printf("Thread 1: %lu, %d is a prime number\n", tid, numVal);
        pthread_exit("is a prime number"); // Exit the thread with a message
    } else {
        printf("Thread 1: %lu, %d is not a prime number\n", tid, numVal);
        pthread_exit("is not a prime number"); // Exit the thread with a message
    }
}
