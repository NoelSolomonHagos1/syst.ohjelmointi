/*
Project 2: Unix Utilities
my-cat.c

Name: Noel Solomon Hagos
Studentnumber: 001379620
*/


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1){ //if no files are given, exit
        return 0;
    }

    char buffer[4096]; // 1024 could work but 4096 is safer

        for (int i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");

            if (fp == NULL) {
                fprintf(stderr, "my-cat: cannot open file\n");
                exit(1);
            }

            while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                printf("%s", buffer);
            }

            fclose(fp);
        }
        return 0;
    
}