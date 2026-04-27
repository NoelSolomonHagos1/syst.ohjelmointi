/*
Project 1: Warmup to C and Unix programming (reverse.c)

Name: Noel Solomon Hagos
Studentnumber: 001379620
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char *line; //Dynamic allocation for line
    struct Node *pNext; //Pointer to the next
} Node;

Node *reverseList(Node *pFirst) { //https://www.geeksforgeeks.org/dsa/reverse-a-linked-list/
    Node *pPrev = NULL;
    Node *pCurrent = pFirst;
    Node *pNext = NULL;
    
    while (pCurrent != NULL) {
        pNext = pCurrent->pNext; //Store next
        pCurrent->pNext = pPrev; //Reverse current node's next pointer
        pPrev = pCurrent;
        pCurrent = pNext;
    }

    return pPrev; 
}

void freeList(Node *pFirst) {
    Node *pCurrent = pFirst;
    while (pCurrent!=NULL) {
        Node *pTemp = pCurrent;
        pCurrent = pCurrent->pNext;
        free(pTemp->line);
        free(pTemp);
    }
}

void printList(Node *pFirst, FILE *output) { //Printing
    Node *pCurrent = pFirst;
    while (pCurrent != NULL) {
        fprintf(output, "%s", pCurrent->line);
        pCurrent = pCurrent->pNext;
    }
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    int inputFile = 0;
    int outputFile = 0;

    if(argc>3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        inputFile = 1;
    }

    if(argc == 3) {
        if(strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        output = fopen(argv[2], "w");
        if(output == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        outputFile = 1;
    }

    Node *pStart = NULL;
    Node *pEnd = NULL;
    char *line = NULL;
    size_t len = 0; 

    while (getline(&line, &len, input) != -1) {
        Node *pNew = malloc(sizeof(Node));
        if (pNew == NULL) {
            fprintf(stderr, "malloc failed\n");
            free(line);
            exit(1);
        }
        pNew->line = malloc(strlen(line) + 1);
        if (pNew->line == NULL) {
            fprintf(stderr, "malloc failed\n");
            free(pNew);
            free(line);
            exit(1);
        }

        strcpy(pNew->line, line);
        pNew->pNext = NULL;

        if(pStart == NULL) {
            pStart = pNew;
            pEnd = pNew;
        } else {
            pEnd->pNext = pNew;
            pEnd = pNew;
        }
    }

    free(line); //free getline
    pStart = reverseList(pStart); //1. reverse the list
    printList(pStart, output); // 2. print the reversed list
    freeList(pStart); 

    if(inputFile){
        fclose(input);
    }
    if(outputFile){
        fclose(output);
    }

    

    return 0;
}