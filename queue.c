#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#define _realloc(p, SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : realloc(p, SIZE))
#define _malloc(SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : malloc(SIZE))
#define MAX_LENGHT_OF_SIMPLE_OPERATION 2

lpQueue CreateQueue(lpQueue pQueue)
{
  pQueue = (lpQueue)malloc(sizeof(queue_t));
  if (pQueue == NULL)
    return NULL;
  pQueue->head = NULL;
  pQueue->tail = NULL;
  return pQueue;
}

void DisplayQueue(lpQueue pQueue)
{
  lpList tmp;
  tmp = pQueue->tail;
  while (tmp)
  {
    if (tmp->symbol.id == LEXID_NUMBER)
      printf("%lf", *tmp->symbol.number);
    else
      printf("%s", tmp->symbol.operation);
    tmp = tmp->next;
  }
}

void Put(lpBool errState, lpQueue pQueue, void *data, lex_id_t id, pFun Fun, pMyFunFirst FunFirst, pMyFunSec FunSec)
{
  lpList newEl = NULL, tmp = NULL;
  char *symbol;
  double *num;
   
  newEl = CreateListEl(newEl);
  if (newEl == NULL)
  {
    *errState = true;
    return;
  }
  
  if (id == LEXID_NUMBER)
  {
    num = (double*)malloc(sizeof(double));
    if (num == NULL)
    {
      *errState = true;
      free(newEl);
      return;
    }
    *num = 0;
    *num = *(double*)data;
    newEl->symbol.number = num;
    newEl->symbol.id = LEXID_NUMBER;
  } 

  else if (id == LEXID_OPEN_BRACKET || id == LEXID_CLOSE_BRACKET || id == LEXID_OPERATION || id == LEXID_COMMA)
  {
    symbol = (char*)malloc(MAX_LENGHT_OF_SIMPLE_OPERATION * sizeof(char));
    if (symbol == NULL)
    {
      *errState = true;
      free(newEl);
      return;
    }
    symbol[MAX_LENGHT_OF_SIMPLE_OPERATION - 1] = 0;
    *symbol = *(char*)data;
    newEl->symbol.operation = symbol;
    newEl->symbol.id = id;
  }
  else if (id >= 1 && id <= 5)
  {
    newEl->symbol.Fun = Fun;
    newEl->symbol.id = id;
  }
  else if (id >= 6 && id <= 11)
  {
    newEl->symbol.FunFirst = FunFirst;
    newEl->symbol.id = id;
  }
  else if (id == 12)
  {
    newEl->symbol.FunSec = FunSec;
    newEl->symbol.id = id;
  }
  if (pQueue->head == NULL)
  {
    pQueue->tail = newEl;
    pQueue->head = newEl;
  }
  else if (pQueue->head != NULL)
  {
    tmp = pQueue->tail;
    while (tmp->next)
      tmp = tmp->next;//get previous el
    tmp->next = newEl;
    pQueue->head = newEl;
  }
}

void ViseVersaQueue(lpQueue pQueue)
{
  lpList tmpT = pQueue->tail, tmp = tmpT->next, tmpH = tmp->next;
  bool_t state = false;

  while (tmpH)
  {
    if (state == false)
    {
      state = true;
      tmpT->next = NULL;
    }
    tmp->next = tmpT;
    tmpT = tmp;
    tmp = tmpH;
    tmpH = tmpH->next;
    if (!tmpH)
    {
      tmp->next = tmpT;
    }
  }
  tmp = pQueue->head;
  pQueue->head = pQueue->tail;
  pQueue->tail = tmp;
}

void DeleteQueue(lpQueue pQueue)
{
  lpList tmpT, tmp;
  
  if (pQueue == NULL)
    return;
  if (pQueue->tail == NULL && pQueue->head == NULL)
  {
    free(pQueue);
    return;
  }
  
  tmp = pQueue->tail->next;
  tmpT = pQueue->tail;
  
  while (tmpT)
  {
    DeleteListEl(tmpT);
    tmpT = tmp;
    if (tmp == NULL)
    {
      free(pQueue);
      return;
    }
    tmp = tmp->next;
  }

  free(pQueue);
}