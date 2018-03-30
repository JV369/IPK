//
// Created by jan on 29.3.18.
//


#include "queue.h"

void QueueInit(TQueue *queue){
    queue->front = NULL;
    queue->end = NULL;
}

void QueueUp(TQueue *queue, long sec, long usec,char *message){
    TQElem newElem = (TQElem)malloc(sizeof(struct QElem));

    newElem->sec = sec;
    newElem->usec = usec;
    newElem->message = (char *)malloc(strlen(message)+1);
    strcpy(newElem->message,message);
    newElem->next = NULL;
    if(queue->front != NULL){
        queue->end->next = newElem;
        queue->end = newElem;
    }
    else{
        queue->front = newElem;
        queue->end = newElem;
    }
}

void QueueFrontPop(TQueue *queue, long *sec, long *usec, char **message){
    TQElem temp = queue->front;
    if(queue->front != queue->end)
        queue->front = queue->front->next;
    else{
        queue->front = NULL;
        queue->end = NULL;
    }

    *sec = temp->sec;
    *usec = temp->usec;
    *message =
    strcpy(*message,temp->message);

    free(temp->message);
    free(temp);
}

void QueueDestroy(TQueue *queue){
    queue->end = NULL;
    while(queue->front != NULL){
        TQElem temp = queue->front;
        queue->front = queue->front->next;
        free(temp->message);
        free(temp);
    }
}