#include <stdlib.h>
#include "stack.h"
#include "RPN.h"

#define IS_PLUS_MINUS(operation) (operation == '+' || operation == '-')
#define IS_MULT_DIV(operation) (operation == '/' || operation == '*')

bool_t _isPriorityOfCurrentHigher(lpStack pStack, lpList curEl);
void _getQueuePriority(lpQueue pStartQueue);
void _copyPriority(lpList to, lpList from);
bool_t _isLeftAssoc(lpList tmpQ, lpList tmpS);
void _bracketError(lpQueue pQueue, lpBool errState);

/*This function take start queue of lexems and returns Reverse Polish notation of expression as a queue of lexems*/
lpQueue RPN(lpQueue pStartQueue, lpBool errState)
{
  lpQueue pReadyQueue = NULL; 
  lpStack pStack = NULL; 
  lpList tmpQ = NULL, tmpS = NULL;
  bool_t isBracket = false;
  
  pReadyQueue = CreateQueue(pReadyQueue);
  if (pReadyQueue == NULL)
  {
    *errState = true;
    DeleteQueue(pStartQueue);
    return NULL;
  }

  pStack = CreateStack(pStack);
  if (pStack == NULL)
  {
    *errState = true;
    free(pReadyQueue);
    DeleteQueue(pStartQueue);
    return NULL;
  }

  _getQueuePriority(pStartQueue);
  
  tmpQ = pStartQueue->tail;
  tmpS = pStack->top;
  
  while (tmpQ)
  {
    if (tmpQ->symbol.id == LEXID_NUMBER)
    {
      //new function
      Put(errState, pReadyQueue, (void*)tmpQ->symbol.number, LEXID_NUMBER, NULL, NULL, NULL);
      if (*errState == true)
      {
        DeleteQueue(pReadyQueue);
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        *errState = true;
        return NULL;
      }
      //
      _copyPriority(pReadyQueue->head, tmpQ);
      tmpQ = tmpQ->next;
    }
    else if (tmpQ->symbol.id == LEXID_OPEN_BRACKET)
    {
      Push(errState, pStack, tmpQ->symbol.operation, LEXID_OPEN_BRACKET, NULL, NULL, NULL);
      if (*errState == true)
      {
        DeleteQueue(pReadyQueue);
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        *errState = true;
        return NULL;
      }
      _copyPriority(pStack->top, tmpQ);
      tmpS = pStack->top;
      tmpQ = tmpQ->next;
    }
    else if (tmpQ->symbol.id >= 0 && tmpQ->symbol.id <= 13)
    { 
      if (_isPriorityOfCurrentHigher(pStack, tmpQ) || 
         (tmpS != NULL && _isLeftAssoc(tmpQ, pStack->top) &&
         tmpQ->symbol.priority <= tmpS->symbol.priority))
      {
        //new function; struct with variable arguments ; push in RPN
        if (tmpQ->symbol.id == 0)
          Push(errState, pStack, (void*)tmpQ->symbol.operation, tmpQ->symbol.id, NULL, NULL, NULL);
        else if (tmpQ->symbol.id >= 1 && tmpQ->symbol.id <= 5)
          Push(errState, pStack, NULL, tmpQ->symbol.id, tmpQ->symbol.Fun, NULL, NULL);
        else if (tmpQ->symbol.id >= 6 && tmpQ->symbol.id <= 11)
          Push(errState, pStack, NULL, tmpQ->symbol.id, NULL, tmpQ->symbol.FunFirst, NULL);
        else if (tmpQ->symbol.id == 12)
          Push(errState, pStack, NULL, tmpQ->symbol.id, NULL, NULL, tmpQ->symbol.FunSec);
        if (*errState == true)
        {
          DeleteQueue(pReadyQueue);
          DeleteStack(pStack);
          DeleteQueue(pStartQueue);
          *errState = true;
          return NULL;
        }
        _copyPriority(pStack->top, tmpQ);
      }
      else if (tmpS != NULL && !_isLeftAssoc(tmpQ, pStack->top) && tmpQ->symbol.priority <= tmpS->symbol.priority)
      {
        while (tmpS != NULL && tmpQ->symbol.priority <= tmpS->symbol.priority)
        {
          if (tmpS->symbol.id == 0)
            Put(errState, pReadyQueue, (void*)tmpS->symbol.operation, tmpS->symbol.id, NULL, NULL, NULL);
          else if (tmpS->symbol.id >= 1 && tmpS->symbol.id <= 5)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, tmpS->symbol.Fun, NULL, NULL);
          else if (tmpS->symbol.id >= 6 && tmpS->symbol.id <= 11)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, tmpS->symbol.FunFirst, NULL);
          else if (tmpS->symbol.id == 12)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, NULL, tmpS->symbol.FunSec);
          if (*errState == true)
          {
            DeleteQueue(pReadyQueue);
            DeleteStack(pStack);
            DeleteQueue(pStartQueue);
            *errState = true;
            return NULL;
          }
          _copyPriority(pReadyQueue->head, tmpS);
          tmpS = tmpS->next;
          Pop(pStack);
        }
        if (tmpQ->symbol.id == 0)
          Push(errState, pStack, (void*)tmpQ->symbol.operation, tmpQ->symbol.id, NULL, NULL, NULL);
        else if (tmpQ->symbol.id >= 1 && tmpQ->symbol.id <= 5)
          Push(errState, pStack, NULL, tmpQ->symbol.id, tmpQ->symbol.Fun, NULL, NULL);
        else if (tmpQ->symbol.id >= 6 && tmpQ->symbol.id <= 11)
          Push(errState, pStack, NULL, tmpQ->symbol.id, NULL, tmpQ->symbol.FunFirst, NULL);
        else if (tmpQ->symbol.id == 12)
          Push(errState, pStack, NULL, tmpQ->symbol.id, NULL, NULL, tmpQ->symbol.FunSec);
        if (*errState == true)
        {
          DeleteQueue(pReadyQueue);
          DeleteStack(pStack);
          DeleteQueue(pStartQueue);
          *errState = true;
          return NULL;
        }
        _copyPriority(pStack->top, tmpQ);
      }
      tmpQ = tmpQ->next;
      tmpS = pStack->top;
    }
    else if (tmpQ->symbol.id == LEXID_CLOSE_BRACKET || (tmpQ->symbol.id == LEXID_COMMA))
    {
      if (tmpQ->symbol.id == LEXID_CLOSE_BRACKET)
        isBracket = true;
      if (tmpS == NULL)
      {
        *errState = true;
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        _bracketError(pReadyQueue, errState);
        return pReadyQueue;
      }
      tmpQ = tmpQ->next;
      while (tmpS && tmpS->symbol.id != LEXID_OPEN_BRACKET)
      {
        if (tmpS->symbol.id == 0)
          Put(errState, pReadyQueue, (void*)tmpS->symbol.operation, tmpS->symbol.id, NULL, NULL, NULL);
        else if (tmpS->symbol.id >= 1 && tmpS->symbol.id <= 5)
          Put(errState, pReadyQueue, NULL, tmpS->symbol.id, tmpS->symbol.Fun, NULL, NULL);
        else if (tmpS->symbol.id >= 6 && tmpS->symbol.id <= 11)
          Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, tmpS->symbol.FunFirst, NULL);
        else if (tmpS->symbol.id == 12)
          Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, NULL, tmpS->symbol.FunSec);
        else if (tmpS->symbol.id == 13)
          Put(errState, pReadyQueue, (void*)tmpS->symbol.operation, tmpS->symbol.id, NULL, NULL, NULL);
        if (*errState == true)
        {
          DeleteQueue(pReadyQueue);
          DeleteStack(pStack);
          DeleteQueue(pStartQueue);
          *errState = true;
          return NULL;
        }
        _copyPriority(pReadyQueue->head, tmpS);
        //test
        //if (tmpS->symbol.id == LEXID_CLOSE_BRACKET)
        Pop(pStack);
        tmpS = pStack->top;
      }
      
      if (tmpS == NULL)
      {
        *errState = true;
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        _bracketError(pReadyQueue, errState);
        return pReadyQueue;
      }
      if (isBracket)
      {
        Pop(pStack);
        tmpS = pStack->top;
        //test
        if (tmpS && tmpS->symbol.id <= 12 && tmpS->symbol.id >= 1)
        {
          if (tmpS->symbol.id >= 1 && tmpS->symbol.id <= 5)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, tmpS->symbol.Fun, NULL, NULL);
          else if (tmpS->symbol.id >= 6 && tmpS->symbol.id <= 11)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, tmpS->symbol.FunFirst, NULL);
          else if (tmpS->symbol.id == 12)
            Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, NULL, tmpS->symbol.FunSec);
          _copyPriority(pReadyQueue->head, tmpS);
          Pop(pStack);//for log(
          tmpS = pStack->top;
        }
        isBracket = false;
      }
    }
  }
  if (tmpQ == NULL && tmpS != NULL)
    while (tmpS)
    {
      if (tmpS->symbol.id == 0)
        Put(errState, pReadyQueue, (void*)tmpS->symbol.operation, tmpS->symbol.id, NULL, NULL, NULL);
      else if (tmpS->symbol.id >= 1 && tmpS->symbol.id <= 5)
        Put(errState, pReadyQueue, NULL, tmpS->symbol.id, tmpS->symbol.Fun, NULL, NULL);
      else if (tmpS->symbol.id >= 6 && tmpS->symbol.id <= 11)
        Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, tmpS->symbol.FunFirst, NULL);
      else if (tmpS->symbol.id == 12)
        Put(errState, pReadyQueue, NULL, tmpS->symbol.id, NULL, NULL, tmpS->symbol.FunSec);
      else if (tmpS->symbol.id == 13)
        Put(errState, pReadyQueue, (void*)tmpS->symbol.operation, tmpS->symbol.id, NULL, NULL, NULL);
      else 
      {
        *errState = true;
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        _bracketError(pReadyQueue, errState);
        return pReadyQueue;
      }
      if (*errState == true)
      {
        DeleteQueue(pReadyQueue);
        DeleteStack(pStack);
        DeleteQueue(pStartQueue);
        *errState = true;
        return NULL;
      }
      _copyPriority(pReadyQueue->head, tmpS);
      tmpS = tmpS->next;
      Pop(pStack);
    }
  DeleteStack(pStack);
  DeleteQueue(pStartQueue);
  _bracketError(pReadyQueue, errState);
  if (pReadyQueue->tail == NULL)
    *errState = true;
  return pReadyQueue;
}

void _bracketError(lpQueue pQueue, lpBool errState)
{
  lpList el = pQueue->tail;
  while (el)
  {
    if (el->symbol.priority == PRIORITY_BRACKETS || el->symbol.id == LEXID_CLOSE_BRACKET || el->symbol.id == LEXID_OPEN_BRACKET)
      *errState = true;
    el = el->next;
  }
}
bool_t _isLeftAssoc(lpList tmpQ, lpList tmpS)
{
  //???
  if (tmpQ->symbol.id == LEXID_OP_LOG)
    return true;
  //???
  if (tmpQ->symbol.priority == tmpS->symbol.priority && (tmpQ->symbol.priority == PRIORITY_UNARY || tmpQ->symbol.priority == PRIORITY_POW || tmpQ->symbol.priority == PRIORITY_UNARY_FUN))
    return true;
  else if ((tmpQ->symbol.priority == PRIORITY_UNARY || tmpQ->symbol.priority == PRIORITY_POW) && (tmpS->symbol.priority == PRIORITY_UNARY || tmpS->symbol.priority == PRIORITY_POW))
    return true;
  else
    return false;
}

/*This function returns state = false if in the stack there is operation that have bigger or equal priority*/
bool_t _isPriorityOfCurrentHigher(lpStack pStack, lpList curEl)
{
  lpList tmpEl = pStack->top;
  if (pStack->top == NULL)
    return true;
  if (!(tmpEl->symbol.priority < curEl->symbol.priority))
    return false;
  return true;
}

/*This function distributes priorities to expression's lexems*/
void _getQueuePriority(lpQueue pStartQueue)
{
  lpList tmpQ = NULL, prevLex = NULL;
  
  if (pStartQueue == NULL)
    return;

  tmpQ = pStartQueue->tail;

  while (tmpQ)
  {
    if (tmpQ->symbol.id == LEXID_NUMBER)
      tmpQ->symbol.priority = -1;

    else if (tmpQ->symbol.id == LEXID_CLOSE_BRACKET || tmpQ->symbol.id == LEXID_OPEN_BRACKET)
      tmpQ->symbol.priority = PRIORITY_BRACKETS;
    else if (tmpQ->symbol.id == LEXID_COMMA)
      tmpQ->symbol.priority = PRIORITY_BRACKETS;

    else if (prevLex != NULL && tmpQ->symbol.id == LEXID_OPERATION && (prevLex->symbol.id == LEXID_CLOSE_BRACKET || prevLex->symbol.id == LEXID_NUMBER))
    {
      if (IS_PLUS_MINUS(*(char*)tmpQ->symbol.operation))
        tmpQ->symbol.priority = PRIORITY_PLUS_MINUS;
      else if (IS_MULT_DIV(*(char*)tmpQ->symbol.operation))
        tmpQ->symbol.priority = PRIORUTY_MULT_DIV;
      else
        tmpQ->symbol.priority = PRIORITY_POW;
    }

    else if (tmpQ->symbol.id >= 1 && tmpQ->symbol.id <= 12 && (prevLex == NULL || prevLex->symbol.id == LEXID_OPEN_BRACKET || prevLex->symbol.id == LEXID_OPERATION))
    {
      tmpQ->symbol.priority = PRIORITY_UNARY_FUN;
    }

    else if (tmpQ->symbol.id == 0 && (prevLex == NULL || prevLex->symbol.id == LEXID_OPEN_BRACKET || prevLex->symbol.id == LEXID_OPERATION))
    {
      tmpQ->symbol.priority = PRIORITY_UNARY;
    }
    //???
    else if (tmpQ->symbol.id == 12)
      tmpQ->symbol.priority = PRIORITY_UNARY_FUN;
    //???
    prevLex = tmpQ;
    tmpQ = tmpQ->next;
  }
}

void _copyPriority(lpList to, lpList from)
{
  to->symbol.priority = from->symbol.priority;
}

