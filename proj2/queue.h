//
// Created by jan on 29.3.18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct QElem{
    char *message;
    long sec;
    long usec;
    struct QElem *next;
} *TQElem;

typedef struct Queue{
    TQElem front;
    TQElem end;
} TQueue;

void QueueInit(TQueue *queue);
void QueueUp(TQueue *queue, long sec, long usec,char *message);
void QueueFrontPop(TQueue *queue, long *sec, long *usec, char **message);
void QueueDestroy(TQueue *queue);
