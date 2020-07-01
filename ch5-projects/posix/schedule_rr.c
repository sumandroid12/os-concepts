#include <stdlib.h>
#include "list.h"
#include "schedulers.h"
#include "task.h"
#include "cpu.h"

struct node *head, *curr_task_node;
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

void pickNextTask()
{
    // In order to delete current node if task completed
    struct node *temp;
    temp = curr_task_node;

    // round robin selection
    if (curr_task_node->next == NULL)
    {
        curr_task_node = head;
    }
    else
    {
        curr_task_node = curr_task_node->next;
    }

    if (temp->task->burst == 0)
    {
        delete (&head, temp->task);
    }
}

void reverse_list()
{
    struct node *rhead, *temp;
    temp = head;
    while (temp != NULL)
    {
        insert(&rhead, temp->task);
        temp = temp->next;
    }
    head = rhead;
}

void schedule()
{
    int slice;
    reverse_list();
    Task *curr_task;
    curr_task_node = head;
    while (head != NULL)
    {
        curr_task = curr_task_node->task;
        slice = QUANTUM <= curr_task->burst ? QUANTUM : curr_task->burst;
        run(curr_task, slice);
        curr_task->burst -= slice;
        pickNextTask();
    }
}