/* References:
 * https://www.daniweb.com/programming/software-development/threads/59077/copy-argv-to-string-in-c-newbie-question
 * https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
 * https://stackoverflow.com/questions/16127027/linux-command-executing-by-popen-on-c-code
 * https://www.daniweb.com/programming/software-development/threads/99538/saving-file-names-from-directory-to-array
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/stat.html
 * https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
 * https://www.geeksforgeeks.org/comparator-function-of-qsort-in-c/
 * */
/* CPSC 457
 * Spring 2018
 * Assignment 2 - Q6
 * file: scan.c
 * usage: ./scan 'suffix' 'numFiles'
 * Name: Siddharth Kataria
 * UCID: 30000880
 * */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

typedef struct inputFile {
    char fileName[MAX_FNAME_SIZE];
    int fileSize;
} inputFile;


int comparator(const void *a, const void *b) {
    struct inputFile *l = (struct inputFile*) a;
    struct inputFile *r = (struct inputFile*) b;
	if(l->fileSize < r->fileSize) 
		return 1;
	return -1;
}


int main( int argc, char ** argv) {
	//Creating variables for arguments
		char suffix[MAX_FNAME_SIZE];
		int N;
	//If the aruguments are not valid:
	if( argc < 3) {
        fprintf(stderr, "Invalid Usage");
        exit(-1);
    }
    else {
		//Taking arguments and putting them into variables
        strcpy(suffix, ".");	//Taking the extension of the file
        strcat(suffix, argv[1]);
        N = atoi(argv[2]);
    }

	//Opening the file
    FILE * fp = popen( "find . -type f", "r");
    if( fp == NULL) {
        perror("popen failed:");
        exit(-1);
    }

    char *fileSuffix;
    char buff[MAX_FNAME_SIZE];
    int nFiles = 0;
    char * files[MAX_FILES];
    //Reading file names
    while(fgets(buff,MAX_FNAME_SIZE,fp)) {
        fileSuffix = strrchr(buff, '.');	
        //To get the index of the file suffix, we substract 1 from the original length
        int fileLen = strlen(fileSuffix) - 1;
        //Checking if file is empty (EOF)
        if(fileSuffix[fileLen] == '\n')
            fileSuffix[fileLen] = '\0';
		//Suffix of file found is the same as out given suffix
        if (strcmp(suffix, fileSuffix) == 0) {
            int finalLen = strlen(buff);
            files[nFiles] = strndup(buff,finalLen);
            nFiles ++;	//Incrementing number of files
        }
    }
    fclose(fp);	//Closing file once reading is complete

    long long totalSize = 0;
    struct stat st;
    struct inputFile finalArray[MAX_FILES];
    for(int i = 0 ; i < nFiles ; i ++ ) {
        if( 0 != stat(files[i],&st)) {
            perror("stat failed:");
            exit(-1);
        }
		//Taking the size (in bytes) of the files we found
        struct inputFile newFile;
        strcpy(newFile.fileName, files[i]);
        newFile.fileSize = st.st_size;	//Setting the file's size
        finalArray[i] = newFile;
		//Adding the size of the file to the total sum
        totalSize += st.st_size;
    }

    //sorting the arrays in decreasing order with the given method
    qsort(finalArray, nFiles, sizeof(struct inputFile), comparator);

    if(N > nFiles) {	//Found less than N number of files with the given suffix
        for(int i = 0; i < nFiles; i++)
			printf("%s %i\n", finalArray[i].fileName, finalArray[i].fileSize);
	}
	else
	{
		for(int i = 0; i < N; i++) 
			printf("%s : %i\n", finalArray[i].fileName, finalArray[i].fileSize);
	}
	
	//Printing total size
    printf("Total size: %lld bytes.\n", totalSize);
    // clean up
    for(int i = 0; i < nFiles ; i ++ ) {
        free(files[i]);
    }
    // return success
    return 0;
}

