#include<stdio.h>
#include<stdlib.h>
#include"mybitmap.h"

#define MIN_PID 300
#define MAX_PID 5000

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

int SIZE;
word_t *bitmap;

int allocate_map(void){
    SIZE=(MAX_PID-MIN_PID)/BITS_PER_WORD + 1;
    bitmap = calloc(SIZE, sizeof(word_t));
}

int allocate_pid(void){
    int count=0;
    while(get_bit(bitmap, count) == 1){ count++; }
    if(count+MIN_PID < MAX_PID){ 
        set_bit(bitmap, count);
        return (count + MIN_PID);
    }
    else{
        fprintf(stderr, " CANNOT ALLOCATE PROCESS\n");
        return -1;
    }
} 
void release_pid(int pid){
    clear_bit(bitmap,pid-MIN_PID);
}

void list_pid(void){
    int count=0;
    while(count+MIN_PID < MAX_PID)
    {
        if(get_bit(bitmap,count)){ printf("%d\n",count+MIN_PID); }
        ++count;
    }
}

int main(void){
    int pid1,pid2;
    allocate_map();
    pid1 = allocate_pid();
    pid2 = allocate_pid();
    for(int i=0;i<400;i++){ allocate_pid();}
    release_pid(301);
    release_pid(302);
    free(bitmap);
}