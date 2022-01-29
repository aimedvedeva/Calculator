#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#define _realloc(p, SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : realloc(p, SIZE))
#define _malloc(SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : malloc(SIZE))
#define MAX_LENGHT_OF_OPERATION 1

lpStack CreateStack(lpStack pStack)
{
  pStack = (lpStack)malloc(sizeof(stack_t));
  if (pStack == NULL)
    return NULL;
  pStack->top = NULL;
  return pStack;
}

void Push(lpBool errState, lpStack pStack, void *data, lex_id_t id, pFun Fun, pMyFunFirst FunFirst, pMyFunSec FunSec)
{
  lpList newEl = NULL;
  char *symbol;//MS2 --->MS3 max_lenght_of_operation(arcctg)
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
  if (id == LEXID_OPEN_BRACKET || id == LEXID_CLOSE_BRACKET || id == LEXID_OPERATION || id == LEXID_COMMA)
  {
    symbol = (char*)malloc(MAX_LENGHT_OF_OPERATION * sizeof(char));
    if (symbol == NULL)
    {
      *errState = true;
      free(newEl);
      return;
    }
    symbol[MAX_LENGHT_OF_OPERATION - 1] = 0;
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
  if (pStack->top == NULL)
    pStack->top = newEl;
  
  else if (pStack->top != NULL)
  {
    newEl->next = pStack->top;
    pStack->top = newEl;
  }
}

void DeleteStack(lpStack pStack)
{
  lpList tmp, tmpT;

  if (pStack->top == NULL)
  {
    free(pStack);
    return;
  }
  tmp = pStack->top;
  tmpT = tmp->next;
  while (tmp)
  { 
    DeleteListEl(tmp);
    tmp = tmpT;
    if (tmp == NULL)
    {
      free(pStack);
      return;
    }
     
    tmpT = tmpT->next;
  }
  free(pStack);
}

void Pop(lpStack pStack)
{
  lpList el = NULL;
  el = pStack->top;
  pStack->top = pStack->top->next;
  DeleteListEl(el);
}

