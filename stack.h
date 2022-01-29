#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "myTypes.h"
#include "list.h"

typedef struct tag_stack
{
  list_t *top;
}stack_t;

typedef stack_t* lpStack;

lpStack CreateStack (lpStack pStack);
void Push(lpBool errState, lpStack pStack, void *data, lex_id_t id, pFun Fun, pMyFunFirst FunFirst, pMyFunSec FunSec);
void Pop (lpStack pStack);
void DeleteStack(lpStack pStack);

#endif