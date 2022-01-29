#ifndef MYTYPES_H_INCLUDED__
#define MYTYPES_H_INCLUDED__

#define _realloc(p, SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : realloc(p, SIZE))
#define _malloc(SIZE) ((((double)rand() / RAND_MAX)) >= 0.75 ? NULL : malloc(SIZE))

typedef enum
{
  false,
  true,
}bool_t;

typedef enum
{
  ERRORTYPE_CORRECT,
  ERRORTYPE_INCORRECT_EXPRESSION,
  ERRORTYPE_LACK_OF_MEMORY,
  ERRORTYPE_NO_MEMORY_FOR_CALCULATING,
  ERROR_TYPE_INCORRECT_DOMAIN_OF_FUN,
  ERRORTYPE_RESULT,
  ERRORTYPE_TOO_MANY_ARGUMENTS,
  ERRORTYPE_FILE_NOT_FOUND,
}error_type_t;

typedef enum
{
  LEXID_OPERATION,
  LEXID_OP_SIN, 
  LEXID_OP_COS, 
  LEXID_OP_FLOOR, 
  LEXID_OP_CEIL, 
  LEXID_OP_ATAN, 
  LEXID_OP_TAN,
  LEXID_OP_CTAN,
  LEXID_OP_ASIN,
  LEXID_OP_ACOS,
  LEXID_OP_LN,
  LEXID_OP_SQRT,
  LEXID_OP_LOG,
  LEXID_NUMBER,
  LEXID_COMMA,
  LEXID_CLOSE_BRACKET,
  LEXID_OPEN_BRACKET,
}lex_id_t;

typedef enum
{
  PRIORITY_BRACKETS,
  PRIORITY_PLUS_MINUS,
  PRIORUTY_MULT_DIV,
  PRIORITY_UNARY,
 // PRIORITY_LOG,
  PRIORITY_POW,
  PRIORITY_UNARY_FUN,
}priotity_t;

typedef bool_t* lpBool;
typedef double(*pMyFunFirst)(bool_t *errState, double x);// pMyFunFirst is a function pointer type
typedef double(*pMyFunSec)(bool_t *errState, double a, double x);
typedef double(*pFun)(double x);

typedef struct{
  double val;
  bool_t isDef;
}tmpLets_t;
typedef struct tag_tokin
{
  lex_id_t id;
  priotity_t priority;
  void *operation;
  pMyFunFirst FunFirst;
  pMyFunSec FunSec;
  pFun Fun;
  double *number;
}lex_t;

#endif

