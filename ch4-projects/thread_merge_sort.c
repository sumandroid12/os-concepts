#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
typedef struct
{
    int low;
    int high;
} data;

void split(int low, int mid, int high);
void sort(data *param);

int arr[N] = {23, 4, 5, 6, 46, 67, 3, 7657, 134, 124};

void sort(data *param)
{
    int low = param->low,
        high = param->high;
    int tmp;
    if (high - low > 1)
        split(low, (low + high) / 2, high);
    else
    {
        if (arr[low] > arr[high])
        {
            tmp = arr[low];
            arr[low] = arr[high];
            arr[high] = tmp;
        }
    }
}
void merge_sorted(int low, int mid, int high)
{
    int n = high - low + 1, i = 0;
    int sub[n];
    int low2 = mid + 1;
    while (i < n)
    {
        if (low2 <= high && low <= mid && arr[low] < arr[low2])
            sub[i++] = arr[low++];
        else if (low2 <= high && low <= mid && arr[low] > arr[low2])
            sub[i++] = arr[low2++];
        else if (low2 <= high)
            sub[i++] = arr[low2++];
        else
            sub[i++] = arr[low++];
    }
    i = 0;
    low = high - n + 1;
    while (i < n)
    {
        arr[low++] = sub[i++];
    }
}
void split(int low, int mid, int high)
{
    data *p1 = (data *)malloc(sizeof(data));
    data *p2 = (data *)malloc(sizeof(data));
    p1->high = mid;
    p1->low = low;
    p2->high = high;
    p2->low = mid + 1;
    sort(p1);
    sort(p2);
    merge_sorted(low, mid, high);
}
void *thread_sort(void *param){ 
    data *d = (data *) param;
    sort(d);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid1,tid2;       /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */
    /* set the default attributes of the thread */
    pthread_attr_init(&attr);
    data *p1 = (data *)malloc(sizeof(data));
    data *p2 = (data *)malloc(sizeof(data));
    p1->high = (N-1)/2;
    p1->low = 0;
    pthread_create(&tid1,&attr,thread_sort,p1);
    p2->high = N-1;
    p2->low = (N-1)/2+1;
    pthread_create(&tid2,&attr,thread_sort,p2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    merge_sorted(p1->low,p1->high,p2->high);
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }
}