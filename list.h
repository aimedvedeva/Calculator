#ifndef LIST_H_INCLUDED__
#define LIST_H_INCLUDED__

#include "myTypes.h"

typedef struct tag_list
{
  lex_t symbol;
  struct tag_list *next;
}list_t;

typedef list_t* lpList;

lpList CreateListEl(lpList newEl);
void DeleteListEl(lpList El);

#endif