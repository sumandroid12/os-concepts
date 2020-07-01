#include <stdlib.h>
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include <limits.h>

struct node
    *priority_q_head[MAX_PRIORITY - MIN_PRIORITY + 1],
    *curr_task_node;
static int tid = 0;

void add(char *name, int priority, int burst)
{
    Task *new_task = malloc(sizeof(Task));
    new_task->tid = tid++;
    new_task->name = name;
    new_task->burst = burst;
    new_task->priority = priority;
    insert(&priority_q_head[priority-1], new_task);
}

void pickNextTask()
{
    struct node *temp, *prev;
    for (int i = 0; i < MAX_PRIORITY - MIN_PRIORITY + 1; i++)
    {
        temp = priority_q_head[i];
        while (temp != NULL)
        {
            if (temp->task->burst != 0)
            {
                curr_task_node = temp;
                return;
            }
            else
            {
                prev = temp;
                temp = temp->next;
                delete (&priority_q_head[i], prev->task);
            }
        }
    }
    curr_task_node = NULL;
}

void schedule()
{
    int slice;
    Task *curr_task;
    pickNextTask();
    while (curr_task_node != NULL)
    {
        curr_task = curr_task_node->task;
        slice = curr_task->burst;
        run(curr_task, slice);
        curr_task->burst -= slice;
        pickNextTask();
    }
}