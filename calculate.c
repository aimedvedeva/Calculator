#include <stdlib.h>
#include <math.h>
#include "stack.h"
#include "RPN.h"

double _getIntermidiateValue(double firstNum, double secNum, lpList elOperation, bool_t *errState);

/*This function should get pReadyQueue and return the result of calculation*/
double Calculate(lpQueue pReadyQueue, lpBool errState)
{
  lpStack pStack = NULL;
  lpList tmpQ = pReadyQueue->tail, tmpS = NULL;
  double *intermidiateRes = (double*)malloc(sizeof(double)), result = 0;
  if (intermidiateRes == NULL)
  {
    *errState = true;
    return 0;
  }
  *intermidiateRes = 0;

  pStack = CreateStack(pStack);
  if (pStack == NULL)
  {
    *errState = true;
    free(intermidiateRes);
    return 0;
  }

  tmpS = pStack->top;
  
  while (tmpQ)
  {
    if (tmpQ->symbol.id == LEXID_NUMBER)
    {
      Push(errState, pStack, tmpQ->symbol.number, tmpQ->symbol.id, NULL, NULL, NULL);
      if (*errState == true)
      {
        free(intermidiateRes);
        DeleteStack(pStack);
        DeleteQueue(pReadyQueue);
        return 0;
      }
      tmpQ = tmpQ->next;
      tmpS = pStack->top;
    }
    else if (tmpQ->symbol.priority != PRIORITY_UNARY && tmpQ->symbol.priority != PRIORITY_UNARY_FUN && tmpQ->symbol.id >= 0 && tmpQ->symbol.id <= 13)//think about log(a,x)!!!
    {
      if (tmpS == NULL || tmpS->next == NULL)
      {
        *errState = true;
        free(intermidiateRes);
        DeleteStack(pStack);
        DeleteQueue(pReadyQueue);
        return 0;
      }
      *intermidiateRes = _getIntermidiateValue(*tmpS->next->symbol.number, *tmpS->symbol.number, tmpQ, errState);
      if (*errState == true)
      {
        free(intermidiateRes);
        DeleteStack(pStack);
        DeleteQueue(pReadyQueue);
        return 0;
      }
      Pop(pStack);
      Pop(pStack);
      Push(errState, pStack, intermidiateRes, LEXID_NUMBER, NULL, NULL, NULL);
      if (*errState == true)
      {
        free(intermidiateRes);
        DeleteStack(pStack);
        DeleteQueue(pReadyQueue);
        return 0;
      }
      tmpS = pStack->top;
      tmpQ = tmpQ->next;
    }
    else if ((tmpQ->symbol.priority == PRIORITY_UNARY || tmpQ->symbol.priority == PRIORITY_UNARY_FUN) && tmpQ->symbol.id >= 0 && tmpQ->symbol.id <= 12)
    {
      if (tmpS == NULL)
      {
        *errState = true;
        free(intermidiateRes);
        DeleteStack(pStack);
        DeleteQueue(pReadyQueue);
        return 0;
      }
      if (tmpQ->symbol.id >= 0 && tmpQ->symbol.id <= 11)
      {
        *intermidiateRes = _getIntermidiateValue(0, *tmpS->symbol.number, tmpQ, errState);
        if (*errState == true)
        {
          free(intermidiateRes);
          DeleteStack(pStack);
          DeleteQueue(pReadyQueue);
          return 0;
        }
        Pop(pStack);
        Push(errState, pStack, intermidiateRes, LEXID_NUMBER, NULL, NULL, NULL);
        if (*errState == true)
        {
          free(intermidiateRes);
          DeleteStack(pStack);
          DeleteQueue(pReadyQueue);
          return 0;
        }
        tmpS = pStack->top;
        tmpQ = tmpQ->next;
      }
      else if (tmpQ->symbol.id == LEXID_OP_LOG)
      {
        *intermidiateRes = _getIntermidiateValue(*tmpS->next->symbol.number, *tmpS->symbol.number, tmpQ, errState);
        if (*errState == true)
        {
          free(intermidiateRes);
          DeleteStack(pStack);
          DeleteQueue(pReadyQueue);
          return 0;
        }
        Pop(pStack);
        Pop(pStack);
        Push(errState, pStack, intermidiateRes, LEXID_NUMBER, NULL, NULL, NULL);
        if (*errState == true)
        {
          free(intermidiateRes);
          DeleteStack(pStack);
          DeleteQueue(pReadyQueue);
          return 0;
        }
        tmpS = pStack->top;
        tmpQ = tmpQ->next;
      }
    }
  }
  if (tmpQ == NULL && tmpS->next != NULL)
  {
    *errState = true;
    free(intermidiateRes);
    DeleteStack(pStack);
    DeleteQueue(pReadyQueue);
    return 0;
  }
  result = *pStack->top->symbol.number;
  free(intermidiateRes);
  DeleteStack(pStack);
  DeleteQueue(pReadyQueue);
  return result;
}

double _getIntermidiateValue(double firstNum, double secNum, lpList elOperation, bool_t *errState)
{
  char *operation = elOperation->symbol.operation;

  if (elOperation->symbol.id == LEXID_OPERATION)
  {
    if (elOperation->symbol.priority != PRIORITY_UNARY)
    {
      switch (*operation)
      {
      case '+':
        return firstNum + secNum;
      case '-':
        return firstNum - secNum;
      case '/':
        if (secNum == 0)
        {
          *errState = true;
          return 0;
        }
        return firstNum / secNum;
      case '*':
        return firstNum * secNum;
      case '^':
        return pow(firstNum, secNum);//test that the secNum is integer!!! 
      }
    }
    else if (elOperation->symbol.priority == PRIORITY_UNARY && *operation == '-')
      return -secNum;
  }
  else
    if (elOperation->symbol.priority == PRIORITY_UNARY_FUN)
    {
      if (elOperation->symbol.id >= 1 && elOperation->symbol.id <= 5)
        return (elOperation->symbol.Fun)(secNum);
      if (elOperation->symbol.id >= 6 && elOperation->symbol.id <= 11)
        return (elOperation->symbol.FunFirst)(errState, secNum);
      if (elOperation->symbol.id == LEXID_OP_LOG)
        return (elOperation->symbol.FunSec)(errState, firstNum, secNum);
    }
  *errState = true;
  return 0;
}