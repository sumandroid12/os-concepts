#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

int sum, min, max;
void *sum_task(void *arr); /* threads call this function */
void *min_task(void *arr); /* threads call this function */
void *max_task(void *arr); /* threads call this function */
int main(int argc, char *argv[])
{
    int arr[7] = {90,81,78,95,79,72,85};
    pthread_t tid1,tid2,tid3;       /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);
    /* create the thread */
    pthread_create(&tid1, &attr, sum_task, arr);
    pthread_create(&tid2, &attr, min_task, arr);
    pthread_create(&tid3, &attr, max_task, arr);
    /* wait for the thread to exit */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    printf("sum = %d∖n", sum);
    printf("min = %d∖n", min);
    printf("max = %d∖n", max);
}
/* The thread will execute in this function */
void *sum_task(void *arr)
{ 
    sum = 0;
    int *list = (int *) arr;
    for (int i = 0; i < 7; i++)
    {
        sum += *list;
        list++;
    }
    pthread_exit(0);
} 

void *max_task(void *arr)
{ 
    max = 0;
    int *list = (int *) arr;
    for (int i = 0; i < 7; i++)
    {
        max = *list > max ? *list: max;
        list++;
    }
    pthread_exit(0);
}

void *min_task(void *arr)
{ 
    min = __INT_MAX__;
    int *list = (int *) arr;
    for (int i = 0; i < 7; i++)
    {
        min = *list < min ? *list: min;
        list++;
    }
    pthread_exit(0);
}
