#include <stdlib.h>
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include <limits.h>

struct node
    *priority_q_head[MAX_PRIORITY - MIN_PRIORITY + 1],
    *curr_task_node;
int curr_task_priority = 0;
static int tid = 0;

void add(char *name, int priority, int burst)
{
    Task *new_task = malloc(sizeof(Task));
    new_task->tid = tid++;
    new_task->name = name;
    new_task->burst = burst;
    new_task->priority = priority;
    insert(&priority_q_head[priority - 1], new_task);
}

void pickNextTask()
{
    struct node *temp, *prev;
    prev = curr_task_node;
    if(curr_task_node->task->burst == 0)
        delete(&priority_q_head[curr_task_priority], curr_task_node->task);

    for (int i = 0; i < MAX_PRIORITY - MIN_PRIORITY + 1; i++)
    {
        if (priority_q_head[i] == NULL)
            continue;
        else{
            if(curr_task_node->next == NULL){
                curr_task_node = priority_q_head[i];
                curr_task_priority = i;
                return;
            }
            else
            {
                curr_task_node = curr_task_node->next;
                curr_task_priority = i;
                return;
            } 
        }
    }
    curr_task_node = NULL;
}

void schedule()
{
    int slice;
    Task *curr_task;
    curr_task_node = priority_q_head[0];
    while (curr_task_node != NULL)
    {
        curr_task = curr_task_node->task;
        slice = QUANTUM <= curr_task->burst ? QUANTUM : curr_task->burst;
        run(curr_task, slice);
        curr_task->burst -= slice;
        pickNextTask();
    }
}