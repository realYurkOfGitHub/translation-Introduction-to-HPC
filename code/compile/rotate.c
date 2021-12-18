/****************************************************************
 ****
 **** This program source is part of
 **** Introduction to High-performance Scientific Computing
 **** by Victor Eijkhout
 **** copyright Victor Eijkhout 2011-2020
 ****
 **** rotate.c : simple program to play with compiler optimizations
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//codesnippet givensfun
void rotate(double *x,double *y,double alpha) {
  double x0 = *x, y0 = *y;
  *x = cos(alpha) * x0 - sin(alpha) * y0;
  *y = sin(alpha) * x0 + cos(alpha) * y0;
  return;
}
//codesnippet end

#include <sys/time.h>

double mysecond()
{
        struct timeval tp;
        //struct timezone tzp;
        int i;

        i = gettimeofday(&tp,NULL /* &tzp */);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

#define NREPS 10000000

int main() {
  
  double x=.5, y=.5, alpha=1.57;
  double start = mysecond();
  for (int i=0; i<NREPS; i++)
    rotate(&x,&y,alpha);
  double duration = mysecond()-start;
  printf("Done after %e\n",duration);
  //  printf("%e\n",x);

  return 0;
}
