/*
Project 5: Parallel zip (pzip.c)
Name: Noel Solomon Hagos
Studentnumber: 001379620
*/

//Sources:
// https://stackoverflow.com/questions/28288775/how-do-you-properly-use-the-fstat-function-and-what-are-its-limits
// https://stackoverflow.com/questions/26259421/use-mmap-in-c-to-write-into-memory
// https://embeddedartistry.com/blog/2017/03/22/memset-memcpy-memcmp-and-memmove/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
 
typedef struct { //Thread control
    char *data; // file data
    char *output; // zipped file
    long start; // start of the thread
    long end; // end of the thread
    long outputSize; // output (not zipped)
} fArgs; 
 
void *compress(void *arg) { //compressing
    fArgs *args = (fArgs *)arg;
 
    long start = args->start;
    long end = args->end;
    char *data = args->data;
 
    // 5 bytes (4-byte int + 1 char)
    args->output = malloc((end - start) * 5);
    if (args->output == NULL) {
        return NULL;
    }
 
    long index = 0;
    long i = start;
 
    //Same logic as the my-zip project
    while (i < end) {
        char curr = data[i];
        int count = 1;
        while (i + count < end && data[i + count] == curr) {
            count++;
        }
        memcpy(args->output + index, &count, sizeof(int));
        index += sizeof(int);
        args->output[index] = curr;
        index++;
        i += count;
    }
    args->outputSize = index;
    return NULL;
}
 
int main(int argc, char *argv[]) {
    if (argc < 2) { //Not in instructions, but similar to my-zip
        printf("pzip: file1 [file2 ...]\n");
        exit(1);
    }
 
    int nprocs = get_nprocs();
    
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);
            continue;
        }
 
        //size of the file using stat st (fstat)
        struct stat st;
        fstat(fd, &st);
        long size = st.st_size;
 
        if (size == 0) {
            close(fd);
            continue;
        }
 
        //memory-map with mmap()
        char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
 
        int currThreads = nprocs;
        if(size<currThreads) {
            currThreads = (size>0)?size:1;
        }
 
        //Dividing files for threading
        long divFile = size / currThreads;
        long rem = size % currThreads;
 
        pthread_t *thread = malloc(currThreads * sizeof(pthread_t));
        fArgs *args = malloc(currThreads * sizeof(fArgs));
 
        //fArgs *workingArgs[nprocs];
 
        //Creating the threads
        for (int j = 0; j < currThreads; j++) {
            args[j].data = data;
            args[j].start = j * divFile + (j<rem?j:rem);
            args[j].end = args[j].start+divFile+(j<rem?1:0);
            // https://pages.cs.wisc.edu/%7Eremzi/OSTEP/threads-api.pdf (page 3)
            pthread_create(&thread[j], NULL, compress, &args[j]);
        }
        
        // https://pages.cs.wisc.edu/%7Eremzi/OSTEP/threads-api.pdf (page 3-5)
        for (int j=0; j<currThreads; j++) {
            pthread_join(thread[j], NULL);
        }
 
        int scan = 1;
        do {
            scan = 0;
            for (int j = 0; j < currThreads - 1; j++) {
                // Skip empty outputs
                if(args[j].outputSize < (int)(sizeof(int) + 1)) {
                    continue;
                }

                // Find next valid output block
                int k = j+1;
                while (k<currThreads && args[k].outputSize < (int)(sizeof(int) + 1)) {
                    k++;
                } 
                if (k >= currThreads) {
                    break; // No more valid blocks to merge with
                }

                // Compare last char of current block with first char of next block
                char last  = args[j].output[args[j].outputSize - 1];
                char first = args[k].output[sizeof(int)];

                if (last == first) {
                    
                    long lastCount = args[j].outputSize - 1 - sizeof(int);
                    int *pc = (int*)(args[j].output + lastCount);
                    *pc += *(int*)args[k].output;

                    // Remove merged pair from next block
                    long r = sizeof(int) + 1;
                    if (args[k].outputSize > r)
                        memmove(args[k].output, args[k].output + r, args[k].outputSize - r);
                    args[k].outputSize -= r;
                    scan = 1;
                }
            }
        }while(scan);
 
        // Printing in order
        for (int j=0; j<currThreads; j++) {
            if (args[j].outputSize > 0) {
                fwrite(args[j].output, 1, args[j].outputSize, stdout);
            }
            free(args[j].output);
        }
 
        free(thread);
        free(args);
        munmap(data,size);
    }
    
    return 0;
}