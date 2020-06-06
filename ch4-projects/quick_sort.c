#include <stdio.h>
#include <stdlib.h>

#define N 10

int arr[N] = {23, 4, 5, 6, 46, 67, 3, 7657, 134, 124};

void swap(int i, int j)
{
    if (i != j)
    {
        int tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
    }
}

void sort(int p, int r)
{
    if (p >= r)
        return;
    int pivot = arr[r];
    int i = p-1, j = p;
    while (j < r)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(i, j);
        }
        j++;
    }
    i++;
    swap(i, r);
    sort(p, i - 1);
    sort(i + 1, r);
}

int main(int argc, char *argv[])
{
    sort(0, N - 1);
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }
}