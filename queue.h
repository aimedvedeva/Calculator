#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "myTypes.h"
#include "list.h"

typedef struct tag_queue
{
  list_t *head;
  list_t *tail;
}queue_t;

typedef queue_t* lpQueue;
lpQueue CreateQueue(lpQueue pQueue);
void Put(lpBool errState, lpQueue pQueue, void *data, lex_id_t id, pFun Fun, pMyFunFirst FunFirst, pMyFunSec FunSec);
void ViseVersaQueue(lpQueue pQueue);
void DeleteQueue(lpQueue pQueue);
void DisplayQueue(lpQueue pQueue);

#endif
