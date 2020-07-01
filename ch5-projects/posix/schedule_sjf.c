
#include <stdlib.h>
#include "list.h"
#include <limits.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"

struct node *head;
static int tid = 0;

void add(char *name, int priority, int burst)
{
    Task *new_task = malloc(sizeof(Task));
    new_task->tid = tid++;
    new_task->name = name;
    new_task->burst = burst;
    new_task->priority = priority;
    insert(&head, new_task);
}

Task *pickNextTask()
{
    if (!head)
        return NULL;
    struct node *temp;
    Task *shortest_task;
    int shortest_burst = INT_MAX;
    temp = head;
    while (temp != NULL)
    {
        if (temp->task->burst < shortest_burst)
        {
            shortest_task = temp->task;
            shortest_burst = temp->task->burst;
        }
        temp = temp->next;
    }
    return shortest_task;
}

void schedule()
{
    Task *next_task = pickNextTask();
    while (next_task)
    {
        int slice = next_task->burst;
        run(next_task, slice);
        delete (&head, next_task);
        next_task = pickNextTask();
    }
}