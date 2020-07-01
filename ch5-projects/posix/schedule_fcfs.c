#include <stdlib.h>
#include "list.h"
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

void reverse_list(){
    struct node *rhead,*temp;
    temp = head;
    while(temp !=NULL){
        insert(&rhead,temp->task);
        delete(&head, temp->task);
        temp = temp->next;
    }
    head = rhead;
}

Task *pickNextTask()
{
    if (!head)
        return NULL;
    struct node *temp;
    Task *next_task;
    temp = head;
    while (temp != NULL)
    {
        next_task = temp->task;
        temp = temp->next;
    }
    return next_task;
}

void schedule()
{
    // reverse_list();
    traverse(head);
    Task *next_task = pickNextTask();
    while (next_task != NULL)
    {
        int slice = next_task->burst;
        run(next_task, slice);
        delete (&head, next_task);
        next_task = pickNextTask();
    }
}