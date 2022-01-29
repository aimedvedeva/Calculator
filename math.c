#include <math.h>
#include "myTypes.h"

#define  M_PI   3.14159265358979323846 /* pi */ 

double Tan(bool_t *errState, double x)
{
  if (cos(x) == 0 || x == M_PI/2)
  {
    *errState = true; 
    return 0;
  }
  else return tan(x);
}
double Ctan(bool_t *errState, double x)
{
  if (sin(x) == 0 || x == M_PI)
  {
    *errState = true;
    return 0;
  }
  else return 1/tan(x);
}
double Asin(bool_t *errState, double x)
{
  if (x <= 1 && x >= -1)
    return asin(x);
  else
  {
    *errState = true;
    return 0;
  }
}
double Acos(bool_t *errState, double x)
{
  if (x <= 1 && x >= -1)
    return acos(x);
  else
  {
    *errState = true;
    return 0;
  }
}
double Sqrt(bool_t *errState, double x)
{
  if (x >= 0)
    return sqrt(x);
  else
  {
    *errState = true;
    return 0;
  }
}
double Ln(bool_t *errState, double x)
{
  if (x > 0)
    return log(x);
  else
  {
    *errState = true;
    return 0;
  }
}
double Log(bool_t *errState, double x, double a)
{
  if (x > 0 && x != 1 && a > 0)
    return log(a)/log(x);
  else 
  {
    *errState = true;
    return 0;
  }
}