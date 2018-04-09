/**
 * Hlavičkový soubor pro frontu
 * Autor: Jan Vávra
 * Login: xvavra20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Fronta pro uložení elementů během měření
 */
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

/**
 * inicializuje frontu pro element QElem
 * @param queue fronta, kterou chceme inicializovat
 */
void QueueInit(TQueue *queue);
/**
 * přidá element do fronty
 * @param queue fronta, do které chceme prvek přidat
 * @param sec čas v sekundách
 * @param usec čas v mikrosekundách
 * @param message přijatá zpráva
 */
void QueueUp(TQueue *queue, long sec, long usec,char *message);
/**
 * vrátí element ze začátku fronty a odstraní ho ze začátku
 * @param queue fronta, ze které chceme výjmout element
 * @param sec ukazatel na proměnou do které se má uložit čas v sekundách
 * @param usec ukazatel na proměnou do které se má uložit čas v mikrosekundách
 * @param message ukazatel na proměnou do které se má uložit zpráva
 */
void QueueFrontPop(TQueue *queue, long *sec, long *usec, char **message);
/**
 * uvolní všechny elementy z fronty
 * @param queue fronta, která se má uvolnit (pro elementy QElem)
 */
void QueueDestroy(TQueue *queue);


/**
 * uložení mezivýsledků měření pro směrodatnou odchylku
 */
typedef struct QElemMeasure{
    float speed;
    struct QElemMeasure *next;
} *TQElemMeasure;


typedef struct QueueMeasure{
    TQElemMeasure front;
    TQElemMeasure end;
} TQueueMeasure;

/**
 * inicializuje frontu pro element QElemMeasure
 * @param queue fronta, kterou chceme inicializovat
 */
void QueueInitMeasure(TQueueMeasure *queue);
/**
 * přidá element do fronty
 * @param queue fronta, do které chceme prvek přidat
 * @param speed naměřená rychlost
 */
void QueueUpMeasure(TQueueMeasure *queue, float speed);
/**
 * vrátí element ze začátku fronty a odstraní ho ze začátku
 * @param queue fronta, ze které chceme výjmout element
 * @param speed ukazatel na proměnou do které se má uložit naměřenou rychlost
 */
void QueueFrontPopMeasure(TQueueMeasure *queue, float *speed);
/**
 * uvolní všechny elementy z fronty
 * @param queue fronta, která se má uvolnit (pro elementy QElemMeasure)
 */
void QueueDestroyMeasure(TQueueMeasure *queue);