#ifndef MATH_H_INCLUDED__
#define MATH_H_INCLUDED__

#define  M_E   2.7182818284590452354  /* e */
#define  M_PI   3.14159265358979323846 /* pi */ 

double Tan(bool_t *errState, double x);
double Ctan(bool_t *errState, double x);
double Asin(bool_t *errState, double x);
double Acos(bool_t *errState, double x);
double Sqrt(bool_t *errState, double x);
double Ln(bool_t *errState, double x);
double Log(bool_t *errState, double a, double x);


#endif