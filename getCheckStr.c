#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "getCheckStr.h"
#include "display.h"
#include "calculate.h"
#include "RPN.h"
#include "queue.h"
#include "list.h"
#include "math.h"


#define MAX_LENGHT_OF_OPERATION 10

#define IS_SIMPLE_OPERATION(x) ((x) == '+' || (x) == '-' || (x) == '-' || (x) == '*' || (x) == '/' || (x) == '^')
#define MAX_SIZE_OF_LENGHT 20

double tmpRes;
char tmpLet;
tmpLets_t tmpLets[123];
bool_t isEx = false;
void _checkStr(char* str);
int _mathFun(lpQueue pQueueOfOneString, char *tmpStr, bool_t *errState, bool_t *isLog);

void GetStr(int argc, char* argv[])
{
  int count = 0, size = MAX_SIZE_OF_LENGHT;
  char *mas = NULL, *tmp = NULL, *tmpmas = NULL;
  FILE * stream = NULL;
  bool_t errState = false;
  int c;
  
  if (argc == 1)
    stream = stdin;
  
  if (argc == 2)
  {
    fopen_s(&stream, argv[1], "r");
    if (stream == NULL)
      Display(NULL, ERRORTYPE_FILE_NOT_FOUND);
  }

  if (argc > 2)
    Display(NULL, ERRORTYPE_TOO_MANY_ARGUMENTS);
  
  while ((c = (int)fgetc(stream)) != EOF)
  {
    mas = (char*)malloc(size*sizeof(char));
    if (c == '\n')
    {
      printf("\n");
      free(mas);
      continue;
    }
    if (mas == NULL)
    {
      Display(NULL, ERRORTYPE_LACK_OF_MEMORY);
      while ((c = (int)fgetc(stream)) != EOF)
      {
        if (c == '\n')
          break;
      }
      if (c == '\n')
        continue;
      else if (c == EOF)
        break;
    }
    else
      tmpmas = mas;
    while (c != '\n')
    {//c:\>a.exe<int.txt
      count++;
      if (c == -1)
        break;
      if (c != '\n' && count == size)
      {
        tmp = realloc(mas, (size)+MAX_SIZE_OF_LENGHT);
        if (tmp)
          mas = tmp;
        else
        {
          free(mas);
          mas = (char*)malloc(size*sizeof(char));
          Display(NULL, ERRORTYPE_LACK_OF_MEMORY);
          while ((c = (int)fgetc(stream)) != '\n');
          errState = true;
          break;
        }
        tmpmas = mas;
        tmpmas += count - 1;
        size += MAX_SIZE_OF_LENGHT;
      }
      *tmpmas = (char)c;
      c = (int)fgetc(stream);
      tmpmas++;
    }
    count = 0;
    *(tmpmas) = 0;
    if (!errState)
      _checkStr(mas);
    else
      errState = false;
    tmpmas = mas;
    free(mas);
  }
}
bool_t _isEqualNumOfBrackets(lpQueue pQueue)
{
  list_t* tmp = pQueue->tail;
  int counterOP = 0, counterCL = 1;
  while (tmp->next)
  {
    if (tmp->symbol.id == LEXID_OPEN_BRACKET)
      counterOP++;
    if (tmp->symbol.id == LEXID_CLOSE_BRACKET)
      counterCL++;
    tmp = tmp->next;
  }
  if (counterCL == counterOP)
    return true;
  else
    return false;
}
int _semicolon(char *tmpStr, lpBool errState)
{
  char *tmp = tmpStr;
  int countS = 0, countE = 0;
  while (*tmp != '\0')
  {
    if (*tmp == ';')
      countS++;
    if (*tmp == '=')
      countE++;
    tmp++;
  }
  if (countE != countS)
    *errState = true;
  return countS;
}

void _errorDone(lpQueue pQueueOfOneString, char *str,long double *num, char* c, char ** tmpNum, lpBool errState, lpBool isLog)
{
  Display(str, ERRORTYPE_NO_MEMORY_FOR_CALCULATING);
  if (pQueueOfOneString != NULL)
    DeleteQueue(pQueueOfOneString);
  free(num);
  free(c);
  free(tmpNum);
  free(errState);
  free(isLog);
  return;
}
void _initStaticVar()
{
  int i;
  isEx = false;
  tmpRes = 0;
  tmpLet = 0;

  for (i = 0; i < 123; i++)
  {
    tmpLets[i].val = 0;
    tmpLets[i].isDef = false;
  }
}
void _checkStr(char* str)
{
  char *tmpStr = NULL, **tmpNum = malloc(sizeof(char*)), *c = malloc(sizeof(char)), cc = 0;
  lpQueue pQueueOfOneString = NULL;
  bool_t flag = false, isEq = false;
  double result = 0;
  long double *num = (long double*)malloc(sizeof(long double));
  lpBool errState = (lpBool)malloc(sizeof(bool_t)), isLog = (lpBool)malloc(sizeof(bool_t));
  int countLog = 0, semicolonCounter = 0, countE = 0;
  
  if (tmpNum == NULL || c == NULL || num == NULL || errState == NULL || isLog == NULL)
  {
    _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
    return;
  }

  //init
  *errState = false;
  *isLog = false;
  _initStaticVar();

  //for using strtod
  *c = cc;
  *tmpNum = c;
  *num = 0;
  
  tmpStr = str;
  
  while (isspace(*tmpStr))
    tmpStr++;
  
  if (*tmpStr == '/' && *(tmpStr + 1) == '/')
   Display(str, ERRORTYPE_CORRECT);
  
  else if (*tmpStr == '\0')
    Display(str, ERRORTYPE_CORRECT);

  else if (*tmpStr == '/' && *(tmpStr + 1) != '/')
    Display(str, ERRORTYPE_INCORRECT_EXPRESSION);

  else if (*tmpStr == ')')
    Display(str, ERRORTYPE_INCORRECT_EXPRESSION);

  else if(*tmpStr == '+' || *tmpStr == '*' || *tmpStr == '/' || *tmpStr == '^')
    Display(str, ERRORTYPE_INCORRECT_EXPRESSION);

  else if (!isalpha(*tmpStr) && !isdigit(*tmpStr) && (*tmpStr == ',' || *tmpStr == '!' || *tmpStr == '"' ||
    *tmpStr == '#' || *tmpStr == '$' || *tmpStr == '%' ||
    *tmpStr == '&' || *tmpStr == ':' || *tmpStr == ';' ||
    *tmpStr == '<' || *tmpStr == '>' || *tmpStr == '=' ||
    *tmpStr == '?' || *tmpStr == '@' || *tmpStr == '_' || (int)*tmpStr == 92 ||
    *tmpStr == ']' || *tmpStr == '[' || (int)(*tmpStr) == 39 ||
    ((int)*tmpStr >= 65 && (int)*tmpStr <= 90)
    || (int)(*tmpStr) == 96 ||
    *tmpStr == '{' || *tmpStr == '}' || *tmpStr == '~' ||
    *tmpStr == '`' || *tmpStr == '|'))
    Display(str, ERRORTYPE_INCORRECT_EXPRESSION);

  else if (isdigit(*tmpStr) || *tmpStr == '(' || *tmpStr == '-' || *tmpStr == '.' || isalpha(*tmpStr))
  {
    pQueueOfOneString = CreateQueue(pQueueOfOneString);
    
    if (pQueueOfOneString == NULL)
    {
      _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
      return;
    }
    semicolonCounter = _semicolon(tmpStr, errState);
    //test g=
    if (*errState == true)
    {
      _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
      return;
    }
    //ошибки нет, значит можем
    countE = semicolonCounter;
    while (*tmpStr != '\0')
    {
       
       while (isspace(*tmpStr))
         tmpStr++;
       
       if (isdigit(*tmpStr) || *tmpStr == '.')
       {
         tmpStr -= sizeof(char);
         if (*tmpStr == ')')
         {
           _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
           return;
         }
         tmpStr += 1;
         *num = strtod(tmpStr, tmpNum);
         Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
         if (tmpStr == *tmpNum)
         {
           _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
           return;
         }
         tmpStr = *tmpNum;
         if (*isLog == true && *tmpStr != ',' && *tmpStr != ')' && *tmpStr != ' ' && !IS_SIMPLE_OPERATION(*tmpStr))
         {
           _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
           return;
         }
       }
       
       //если число е встречаетс€ в выражении или просто в примере -> в стек
       if ((*tmpStr == 'e' && isEx && countE != 0 && semicolonCounter > 0) || (*tmpStr == 'e' && (!isEx) && countE == 0 && semicolonCounter == 0))// Ќ≈ ƒќ∆≈Ќ «ј’ќƒ»“№ ≈—Ћ» ISEX == FALSE && ISeQ == TRUE SEMOcOLON > 0
       {
         *num = M_E;
          Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
         if (*errState == true)
         {
           _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
           return;
         }
         tmpStr++;
       }
       
       //pi -> в стек
       if (*tmpStr == 'p' && *(tmpStr + 1) == 'i')
       {
         *num = M_PI;
         Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
        if (*errState == true)
        { 
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr += 2 * sizeof(char);
       }
       
       else if (isalpha(*tmpStr))
       {  
         tmpStr++;
         //если последний сивол в выражении - это буква --- дубль кода!
         if (*tmpStr == 0 && !isEx && isEq)
         {
           tmpStr--;
           if (tmpLets[(int)(*tmpStr)].isDef == true)
           {
             *num = tmpLets[(int)(*tmpStr)].val;
             Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
             tmpStr++;
           }
           else if (*tmpStr == 'e')
           {
             *num = M_E;
             Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
             tmpStr++;
           }
           else
           {
             _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
             return;
           }
         }
         //если следующа€ не буква и не было равно, но кол-во ; не ноль Ц> запоминаем
         else if (!isalpha(*tmpStr) && semicolonCounter != 0 && !isEx)
         {
           tmpStr--;
           tmpLet = *tmpStr;
           tmpStr++;
           isEq = true;
         }
         //если равно уже было + следующа€ буква -> подмена -> в стек
         else if ((!isalpha(*tmpStr) && isEx) || (!isalpha(*tmpStr) && semicolonCounter == 0 && countE != 0 && !isEx && isEq))
         {
           tmpStr--;
           if (tmpLets[(int)(*tmpStr)].isDef == true)
           {
             *num = tmpLets[(int)(*tmpStr)].val;
              Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
             tmpStr++;
           }
           //если в конечном выражении из переменных прибавл€ем букву e, котора€ не инициалищирована, как переменна€
           else if (*tmpStr == 'e')
           {
             *num = M_E;
             Put(errState, pQueueOfOneString, (void*)num, LEXID_NUMBER, NULL, NULL, NULL);
             tmpStr++;
           }
           //если подмена на неинициализированную переменную Ц> ошибка
           else
           {
            _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
            return;
          }
        }
        //если следующа€ буква и (выражение+после занака равно) или (выражени€ нет)
        else if (isalpha(*tmpStr) && ((isEx && countE > 0) || (!isEx)))
        {
          tmpStr--;
          tmpStr += _mathFun(pQueueOfOneString, tmpStr, errState, isLog);
          flag = true;
          if (*isLog)
            countLog++;
          if (*errState == true)
          {
            _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
            return;
          }
        }
        //если следующа€ не буква и это не выражение
        else if (semicolonCounter == 0 && countE == 0 && !isEx && !isEq && !isalpha(*tmpStr))
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        //если никуда не зашли -> вернули обратно
        else
          tmpStr--;
      }
      
      else if (*tmpStr == ';' && isEq == true && isEx == true)
      {
        tmpStr++;
        pQueueOfOneString = RPN(pQueueOfOneString, errState);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpRes = Calculate(pQueueOfOneString, errState);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpLets[(int)(tmpLet)].val = tmpRes;
        tmpLets[(int)tmpLet].isDef = true;
        tmpRes = 0;
        pQueueOfOneString = CreateQueue(pQueueOfOneString);
        isEx = false;
        semicolonCounter--;
      }

      else if (*tmpStr == '=' && isEq == true)
      {
        isEx = true;
        tmpStr++;
      }

      else if (*tmpStr == ',' && countLog > 0)
      {
        *isLog = false;
        tmpStr--;
        //test isalpha in condition
        if (!isdigit(*tmpStr) && *tmpStr != ')' && *tmpStr != ' ' && !isalpha(*tmpStr))
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr++;
        tmpStr++;
        if (!isalpha(*tmpStr) && !isdigit(*tmpStr) && *tmpStr != '(' && *tmpStr != ' ')
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr--;

        Put(errState, pQueueOfOneString, (void*)tmpStr, LEXID_COMMA, NULL, NULL, NULL);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr++;
        countLog--;
      }

      else if (IS_SIMPLE_OPERATION(*tmpStr))
      {
        Put(errState, pQueueOfOneString, (void*)tmpStr, LEXID_OPERATION, NULL, NULL, NULL);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr++;
      }

      else if (*tmpStr == ')')
      {
        //test
        if (*isLog == true && _isEqualNumOfBrackets(pQueueOfOneString))
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr -= sizeof(char);
        if (IS_SIMPLE_OPERATION(*tmpStr))
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr += 1;
        Put(errState, pQueueOfOneString, (void*)tmpStr, LEXID_CLOSE_BRACKET, NULL, NULL, NULL);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr++;
      }

      else if (*tmpStr == '(')
      {
        if (pQueueOfOneString->tail != NULL)
        {
          tmpStr -= sizeof(char);
          if (isdigit(*tmpStr))
          {
            _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
            return;
          }
          tmpStr += 1;
        }
        Put(errState, pQueueOfOneString, (void*)tmpStr, LEXID_OPEN_BRACKET, NULL, NULL, NULL);
        if (*errState == true)
        {
          _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
          return;
        }
        tmpStr++;
        flag = false;
      }

      else if ((*tmpStr == ',' || *tmpStr == '!' || *tmpStr == '"' ||
        *tmpStr == '#' || *tmpStr == '$' || *tmpStr == '%' ||
        *tmpStr == '&' || *tmpStr == ':' || *tmpStr == ';' ||
        *tmpStr == '<' || *tmpStr == '>' || *tmpStr == '=' ||
        *tmpStr == '?' || *tmpStr == '@' || *tmpStr == '_' ||
        *tmpStr == ']' || *tmpStr == '[' || (int)(*tmpStr) == 39 ||
        (int)*tmpStr == 92 || ((int)*tmpStr >= 65 && (int)*tmpStr <= 90)
        || (int)(*tmpStr) == 96 ||
        *tmpStr == '{' || *tmpStr == '}' || *tmpStr == '~' ||
        *tmpStr == '`' || *tmpStr == '|') && (isEx || (!isEx && *tmpStr != '=') || (!isEx && *tmpStr != ';')))
      {
        _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
        return;
      }
     }
     if (flag == true)
     {
       _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
       return;
    }  
 
    pQueueOfOneString = RPN(pQueueOfOneString, errState);
    if (*errState == true)
    {
      _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
      return;
    }

    result = Calculate(pQueueOfOneString, errState);
    if (*errState == true)
    {
      pQueueOfOneString = NULL;
      _errorDone(pQueueOfOneString, str, num, c, tmpNum, errState, isLog);
      return;
    }

    DisplayResult(str, result);
  }
  free(num);
  free(c);
  free(tmpNum);
  free(errState);
  free(isLog);
  return;
}

int _mathFun(lpQueue pQueueOfOneString, char *tmpStr, bool_t *errState, bool_t *isLog)
{
  char *tmp = NULL, *ptmp = NULL;
  int counterShift = 0;

  double (*pMyFunFirst)(bool_t *errState, double x);// pMyFunFirst is a function pointer type
  double (*pMyFunSec)(bool_t *errState, double a, double x);
  double (*pFun)(double x);
 
  tmp = (char*)malloc(MAX_LENGHT_OF_OPERATION * sizeof(char));
  //*isLog = false;
  if (tmp == NULL)
  {
    *errState = true;
    return 0;
  }

  ptmp = tmp;

  while (isalpha(*tmpStr))
  {
    if (counterShift == MAX_LENGHT_OF_OPERATION - 1)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
    *ptmp++ = *tmpStr++;
    counterShift++;
  }

  *ptmp = 0;
  //tmp -= counterShift * sizeof(char);

  if (!strcmp("arcsin", tmp))
  {
    pMyFunFirst = &Asin;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_ASIN, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("sin", tmp))
  {
    pFun = &sin;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_SIN, pFun, NULL, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("cos", tmp))
  {
    pFun = &cos;
    Put(errState ,pQueueOfOneString, NULL, LEXID_OP_COS, pFun, NULL, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("arccos", tmp))
  {
    pMyFunFirst = &Acos;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_ACOS, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("tg", tmp))
  {
    pMyFunFirst = &Tan;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_TAN, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("arctg", tmp))
  {
    pFun = &atan;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_ATAN, pFun, NULL, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("ctg", tmp))
  {
    pMyFunFirst = &Ctan;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_CTAN, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("ln", tmp))
  {
    pMyFunFirst = &Ln;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_LN, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("sqrt", tmp))
  {
    pMyFunFirst = &Sqrt;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_SQRT, NULL, pMyFunFirst, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("log", tmp))
  {
    pMyFunSec = &Log;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_LOG, NULL, NULL, pMyFunSec);
    *isLog = true;
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("ceil", tmp))
  {
    pFun = &ceil;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_CEIL, pFun, NULL, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else if (!strcmp("floor", tmp))
  {
    pFun = &floor;
    Put(errState, pQueueOfOneString, NULL, LEXID_OP_FLOOR, pFun, NULL, NULL);
    if (*errState == true)
    {
      free(tmp);
      *errState = true;
      return 0;
    }
  }
  else
  {
    *errState = true;
    free(tmp);
    return counterShift;
  }
  free(tmp);
  return counterShift;
}
