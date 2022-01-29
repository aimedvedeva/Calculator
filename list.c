#include <stdlib.h>
#include "list.h"

lpList CreateListEl(lpList newEl)
{
  newEl = (lpList)malloc(sizeof(list_t));
  if (newEl == NULL)
    return NULL;
  newEl->next = NULL;
  return newEl;
}

void DeleteListEl(lpList El)
{
  if (El == NULL)
    return;
  else
  {
    if (El->symbol.id == LEXID_CLOSE_BRACKET || El->symbol.id == LEXID_OPEN_BRACKET || El->symbol.id == LEXID_OPERATION || El->symbol.id == LEXID_COMMA)
      free(El->symbol.operation);

    else if (El->symbol.id == LEXID_NUMBER)
      free(El->symbol.number);
    free(El);
  }
}