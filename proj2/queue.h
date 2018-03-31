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



typedef struct QElemMeasure{
    float speed;
    struct QElemMeasure *next;
} *TQElemMeasure;


typedef struct QueueMeasure{
    TQElemMeasure front;
    TQElemMeasure end;
} TQueueMeasure;



void QueueInitMeasure(TQueueMeasure *queue);
void QueueUpMeasure(TQueueMeasure *queue, float speed);
void QueueFrontPopMeasure(TQueueMeasure *queue, float *speed);
void QueueDestroyMeasure(TQueueMeasure *queue);