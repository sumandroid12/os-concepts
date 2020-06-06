#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define N 9

typedef struct
{
    int row;
    int column;
    int *result;
} parameters;

int board[N][N] = {{6, 2, 4, 5, 3, 9, 1, 8, 7},
                   {5, 1, 9, 7, 2, 8, 6, 3, 4},
                   {8, 3, 7, 6, 1, 4, 2, 9, 5},
                   {1, 4, 3, 8, 6, 5, 7, 2, 9},
                   {9, 5, 8, 2, 4, 7, 3, 6, 1},
                   {7, 6, 2, 3, 9, 1, 4, 5, 8},
                   {3, 7, 1, 9, 5, 6, 8, 4, 2},
                   {4, 9, 6, 1, 8, 2, 5, 7, 3},
                   {2, 8, 5, 4, 7, 3, 9, 1, 6}};
int results[11] = {0};

// check whether elements are unique
int validate(int *arr[N])
{
    int hash[N] = {0};
    for (int i = 0; i < N; i++)
    {
        if (++hash[*arr[i] - 1] == 2)
            return 0;
    }
    return 1;
}

void *row_thread(void *result)
{
    int *arr[N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            arr[j] = &board[j][i];
        }
        // check whether each row consists of unique elements
        if (validate(arr) == 0)
        {
            *((int *)result) = 0;
            pthread_exit(0);
        }
    }
    *((int *)result) = 1;
    pthread_exit(0);
}
void *col_thread(void *result)
{
    int *arr[N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            arr[j] = &board[i][j];
        }
        // check whether each column consists of unique elements
        if (validate(arr) == 0)
        {
            *((int *)result) = 0;
            pthread_exit(0);
        }
    }
    *((int *)result) = 1;
    pthread_exit(0);
}

// check whether each grid consists of unique elements
void *grid_thread(void *result)
{
    parameters *data = (parameters *)result;
    int r = data->row;
    int c = data->column;
    int n = 0;
    int *arr[N];
    for (int i = r; i < r + 3; i++)
    {
        for (int j = c; j < c + 3; j++)
        {
            arr[n++] = &board[i][j];
        }
    }
    if (validate(arr) == 0)
    {
        *data->result = 0;
        pthread_exit(0);
    }
    *data->result = 1;
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    /* 11 Threads: 9 for grid 1 for columns 1 for rows */
    pthread_t threads[11];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    /* grid_thread */
    int n = 0;
    for (int i = 0; i < N; i += 3)
    {
        for (int j = 0; j < N; j += 3)
        {
            parameters *data = (parameters *)malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            data->result = &results[n];
            pthread_create(&threads[n++], &attr, grid_thread, data);
        }
    }
    /* row_thread */
    pthread_create(&threads[9], &attr, row_thread, &results[9]);
    /* col_thread */
    pthread_create(&threads[10], &attr, col_thread, &results[10]);
    /* thread creation finished */

    for (int i = 0; i < 11; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < 11; i++)
    {
        if (results[i] == 0)
        {
            printf("Invalid sudoku! commit seppuku!!");
            return 0;
        }
    }
    printf("Proud of u!!");
    return 0;
}