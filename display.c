#include <stdio.h>
#include "display.h"

void Display(char *tmpStr, error_type_t errorType)
{
  char *eq = " == ";
  switch (errorType)
  {
  case ERRORTYPE_CORRECT:
    printf("%s", tmpStr);
    printf("\n");
    break;
  case ERRORTYPE_INCORRECT_EXPRESSION:
    printf("%s", tmpStr);
    printf("%s", eq);
    printf("ERROR: INCORRECT_EXPRESSION");
    printf("\n");
    break;
  case ERROR_TYPE_INCORRECT_DOMAIN_OF_FUN:
    printf("%s", tmpStr);
    printf("%s", eq);
    printf("ERROR: INCORRECT_DOMAIN_OF_FUN");
    printf("\n");
    break;
  case ERRORTYPE_NO_MEMORY_FOR_CALCULATING:
    printf("%s", tmpStr);
    printf("%s", eq);
    printf("ERROR: NO_MEMORY_FOR_CALCULATING");
    printf("\n");
    break;
  case ERRORTYPE_FILE_NOT_FOUND:
    printf("ERROR: File not found");
    printf("\n");
    break;
  case ERRORTYPE_LACK_OF_MEMORY:
    printf("ERROR: ERRORTYPE_LACK_OF_MEMORY");
    printf("\n");
    break;
  case ERRORTYPE_TOO_MANY_ARGUMENTS:
    printf("ERROR: Too many arguments");
    printf("\n");
    break;
  }
}

void DisplayResult(char *tmpStr, double result)
{
  printf("%s", tmpStr);
  printf(" == ");
  printf("%g", result);
  printf("\n");
}

