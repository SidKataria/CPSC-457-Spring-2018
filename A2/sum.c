/* References:
 * https://stackoverflow.com/questions/35061854/creating-multiple-threads-in-c
 * https://codereview.stackexchange.com/questions/106773/dividing-a-range-into-n-sub-ranges
 * https://www.codingforums.com/computer-programming/177466-c-c-reading-text-file-numbers-into-single-dimensional-arrays.html
 * https://codereview.stackexchange.com/questions/106773/dividing-a-range-into-n-sub-ranges
 * https://www.quora.com/How-do-you-read-integers-from-a-file-in-C
 * https://stackoverflow.com/questions/2879537/how-to-use-fgets-if-you-dont-know-the-number-of-characters-to-be-read
 * https://arstechnica.com/civis/viewtopic.php?f=20&t=627504
 * https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
 **/
/* CPSC 457
 * Spring 2018
 * Assignment 2 - Q8
 * file: sum.c
 * compile: gcc sum.c -l pthread -o sum -lm
 * usage: ./sum 'file name(.txt)' 'number of threads'
 * Name: Siddharth Kataria
 * UCID: 30000880
 **/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//Defining final values for file size and number of threads
#define MAX_SIZE 512
#define MAX_THREADS 100000000

//Declaring global variables for pthread file
char fname[MAX_SIZE];	
int givenThreads;
int arrayLen;
int subArray[MAX_THREADS];	//Sub arrays for each thread
int totalSum;

//Declaring the pthread mutex
pthread_mutex_t mutex;

void * calcSum(void * tid) {
    int eleFirst;	//First element
    int eleLast;	//Last element
    int eleRemain;	//element if size of array is odd
    
    //Dividing the array
    float arrayCut = arrayLen / givenThreads;
    
    //Rounding up to the ceiling if length is odd
    int thisLen = ceil(arrayLen);	
    
    int remainLen = thisLen * givenThreads;
    int tSum = 0;
    
    //Converting the thread's ID to a long integer
    long int thisTid = (long int) tid;

	//Dividing the array into equal parts for the threads to calculate sum
    if((remainLen - arrayLen) != 0)
        eleRemain = thisLen - arrayLen;
    else
        eleRemain = thisLen;
    //Checking if the  number of thread IDs match the given number of threads 
	if((thisTid +1) == givenThreads) {
        eleFirst = thisTid * thisLen;
        eleLast = eleFirst + thisLen - eleRemain;
    }
    else {
        eleFirst = thisTid * thisLen;
        eleLast = eleFirst + thisLen;
    }

	//Locking mutex
    pthread_mutex_lock(&mutex);
    //Critical section
    for (int i = eleFirst; i < eleLast; i++)
        tSum = tSum + subArray[i];	
    //End of critical section
    pthread_mutex_unlock(&mutex);	//Unlocking mutex after done calculating its given array sum

	//Printing Each thread with it's ID the sum of its assigned array
    printf("\tThread %ld: %d\n", (thisTid), tSum);

	//Locking mutex
    pthread_mutex_lock(&mutex);
    //Critical section for calculating total sum of the array
    totalSum += tSum;	//Adding each individual array's sum to get total sum
    //End of critical section
    pthread_mutex_unlock(&mutex);	//Unlocking mutex

	//Exit thread after usage
    pthread_exit(0);
}

int main( int argc, char ** argv) {
	//Intializing the pthread mutex
    pthread_mutex_init(&mutex, NULL);

	//Checking for improper usage
    if( argc < 3) {
        fprintf(stderr, "Invalid Usage");
        exit(-1);
    }
    else {
        stpcpy(fname, argv[1]);		//Taking 1st argument as our file name
        givenThreads = atoi(argv[2]);	//Taking 2nd argument as number of threads
    }

	//Opening file
    FILE * fp = fopen(fname, "r");
    if( fp == NULL) {
        perror("popen failed:");
        exit(-1);
    }

	//Declaring a memory buffer
    char buff[MAX_SIZE];
    arrayLen = 0;
    //Reading from file until memory full or EOF
    while(fgets(buff,MAX_SIZE,fp)) {
        //Obtaining an integer
        int gotInt = strlen(buff);
        //Getting integer into memory
        int finalInt = atoi(strndup(buff,gotInt));
        //Adding integer into a sub array for a thread to calcuate its sum
        subArray[arrayLen] = finalInt;
        arrayLen++;	//Incrementing the length of array
    }
    //Closing file once done reading
    fclose(fp);

	//Declaring pthreads for calculation
    pthread_t numThreads[givenThreads];
    totalSum = 0;
    //Creating multiple threads
    for(long i = 0; i < givenThreads; i++) {
        pthread_create(&numThreads[i], NULL, calcSum, (void *) i);
    }
	//Waiting for each thread to finish calculation
    for (int i = 0; i < givenThreads; i++) {
        pthread_join(numThreads[i], NULL);
    }
	
	//Destroying mutex once done using
    pthread_mutex_destroy(&mutex);
    
    //Printing total sum of the original .txt file of integers
    printf("\tSum = %d\n", totalSum);
    
    //End main()
    return 0;
}
