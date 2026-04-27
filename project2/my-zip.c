/*
Project 2: Unix Utilities
my-zip.c

Name: Noel Solomon Hagos
Studentnumber: 001379620
*/


#include <stdio.h>
#include <stdlib.h>

//Inspiration from https://dev.to/farhadrahimiklie/building-a-file-compressor-in-c-from-scratch-2n25

//writes the count and character to stodut

void fWrite(int count, int character) { 
    fwrite(&count, sizeof(int), 1, stdout); // sizeof(int) 4 bytes
    fwrite(&character, sizeof(char), 1, stdout); // sizeof(char) 1 byte
}

//going through the file
void handleFile(FILE *fp, int *prev, int *count) {
    int ch;

    //https://man7.org/linux/man-pages/man3/fgetc.3.html (using fgetc)
    //reads one character at a time
    while ((ch = fgetc(fp)) != EOF) {
        if (*prev == ch) {
            (*count)++;
        }else {
            //Compressing
            if(*count > 0) { 
                fWrite(*count, *prev);
            }
            *prev = ch;
            *count = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    //If no files are specified, exit (needs at least one file)
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    int prev = -1; //-1 meaning no characters read yet
    int count = 0;

    for (int i = 1; i<argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp==NULL) {
            printf("my-zip: cannot open file\n");
            exit(1);
        }
        
        handleFile(fp, &prev, &count);
        fclose(fp);
    }

    if(count>0) {
        fWrite(count, prev);
    }

    return 0;
}