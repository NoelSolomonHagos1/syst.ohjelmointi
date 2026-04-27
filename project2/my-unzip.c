/*
Project 2: Unix Utilities
my-unzip.c

Name: Noel Solomon Hagos
Studentnumber: 001379620
*/


#include <stdio.h>
#include <stdlib.h>

//Inspiration from https://dev.to/farhadrahimiklie/building-a-file-compressor-in-c-from-scratch-2n25
//https://man7.org/linux/man-pages/man3/fread.3.html (fread)
//help source for fread: https://stackoverflow.com/questions/50312089/how-does-fread-in-c-actually-work (fread)

//reads the compressed file and prints the characters
void handleFile(FILE *fp) {
    int count;
    char chart;

    //read 4 bytes for count and 1 byte for character
    //Source: https://overiq.com/c-programming-101/fread-function-in-c/
    while (fread(&count, sizeof(int), 1, fp) == 1 && fread(&chart, sizeof(char), 1, fp) == 1) {
        for(int i = 0; i<count; i++) {
            printf("%c", chart);
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) { //Exit if no files are specified
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    //going through files
    for (int i = 1; i<argc;i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }

        handleFile(fp);
        fclose(fp);
    }

    return 0;
}