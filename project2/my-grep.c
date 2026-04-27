/*
Project 2: Unix Utilities
my-grep.c

Name: Noel Solomon Hagos
Studentnumber: 001379620
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//read file and print lines that contain the search word
void search(FILE *fp, char *searchterm) {
    char *line = NULL;
    size_t len = 0;

    //handling of long lines
    while (getline(&line, &len, fp) != -1) {
        if (strstr(line, searchterm) != NULL) {
            printf("%s", line);
        }
    }
    free(line); //free memory
}

int main(int argc, char *argv[]) {
    if(argc<2) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }
    
    char *searchterm = argv[1];

    //only search term given
    if(argc == 2) {
        search(stdin, searchterm);
        return 0;
    }
 
    //going through files
    for (int i=2; i<argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("my-grep: cannot open file\n");
            exit(1);
        }
        search(fp, searchterm);
        fclose(fp);
    }

    return 0;
}