#include <stdio.h>
#include <stdlib.h>
#include "fr1x16.h"

#define ALLOC_CHUNK 1024

extern void process(void);
extern void init(void);

fract16 inLeft = 0, inRight = 0, outLeft = 0, outRight = 0;
static fract16 *inLeftBuf = NULL, *inRightBuf = NULL;

int main(int argc, char *argv[]){
    unsigned len, i, bufsize;
    FILE *fp;
    
    if(argc == 1){
        fp = fopen("input.dat", "rt");
        if(fp == NULL){
            printf("ERROR: Cannot access input.dat. If no input file is supplied, then the number of simulation steps must be given as a command line argument.\n\n");
            return 1;
        }
        inLeftBuf = (fract16*)malloc(ALLOC_CHUNK * sizeof(fract16));
        inRightBuf = (fract16*)malloc(ALLOC_CHUNK * sizeof(fract16));
        bufsize = ALLOC_CHUNK;
        len = 0;
        while(fscanf(fp, "%hd %hd", &inLeftBuf[len], &inRightBuf[len]) != EOF){
            len++;
            if(len == bufsize){
                bufsize += ALLOC_CHUNK;
                inLeftBuf = (fract16*)realloc(inLeftBuf, bufsize * sizeof(fract16));
                inRightBuf = (fract16*)realloc(inRightBuf, bufsize * sizeof(fract16));
            }
        }
        fclose(fp);
    }
    else if(argc == 2){
        len = atoi(argv[1]);
        inLeftBuf = (fract16*)calloc(len, sizeof(fract16));
        inRightBuf = (fract16*)calloc(len, sizeof(fract16));
    }
    else{
        printf("ERROR: invalid number of input arguments (%d).\n\n", argc);
        return 1;        
    }

    fp = fopen("output.dat", "wt");
    if(fp == NULL){
        printf("ERROR: Cannot open output.dat.");
        free(inLeftBuf);
        free(inRightBuf);
        return 1;
    }
    init();
    for(i=0; i<len; i++){
        inLeft = inLeftBuf[i];
        inRight = inRightBuf[i];
        process();
        fprintf(fp, "%hd %hd\n", outLeft, outRight);
    }
    fclose(fp);
    free(inLeftBuf);
    free(inRightBuf);
    return 0;
}
