/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

/*
 * File:    inner1.c
 * Author:  Victor Eijkhout <eijkhout@tacc.utexas.edu>
 *
 * Usage: inner1 n
 */

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

#define PCHECK(e) if (e!=PAPI_OK) {printf("Problem in papi call, line %d\n",__LINE__); return 1;}
#define NEVENTS 2
#define MAXN 100000

/* we are assuming array storage in C row mode */
#if defined(SHIFT)
#define INDEX(i,j,m,n) (i)*(n+8)+(j)
#else
#define INDEX(i,j,m,n) (i)*(n)+(j)
#endif

int main(int argc, char **argv)
{
   int events[NEVENTS] = {PAPI_L1_DCM,PAPI_TOT_CYC}; long_long values[NEVENTS];
   PAPI_event_info_t info, info1;
   const PAPI_hw_info_t *hwinfo = NULL;
   int retval,event_code, m,n, i,j;
   const PAPI_substrate_info_t *s = NULL;
   double A[MAXN+4],B[MAXN+4];
   double x=0.;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   {
     int i,n; double *a=A,*b=B;
     double sum1;
     double temp1;

     sum1 = 0.; temp1 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       a++;
     }

     sum1 += temp1;

     x += sum1;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("1.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   {
     int i,n; double *a=A,*b=B;
     double sum1, sum2;
     double temp1, temp2;

     sum1 = 0.; sum2 = 0.;
     temp1 = 0.; temp2 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN/2-1; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       sum2 += temp2;
       temp2 = *(a + 1) * *(b + 1);

       a += 2; b += 2;
     }

     sum1 += temp1;
     sum2 += temp2;

     x += sum1 + sum2;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("2.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   {
     int i,n; double *a=A,*b=B;
     double sum1, sum2, sum3;
     double temp1, temp2, temp3;

     sum1 = 0.; sum2 = 0.; sum3 = 0.;
     temp1 = 0.; temp2 = 0.; temp3 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN/3-1; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       sum2 += temp2;
       temp2 = *(a + 1) * *(b + 1);

       sum3 += temp3;
       temp3 = *(a + 2) * *(b + 2);

       a += 3; b += 3;
     }

     sum1 += temp1;
     sum2 += temp2;
     sum3 += temp3;

     x += sum1 + sum2 + sum3;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("3.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   {
     int i,n; double *a=A,*b=B;
     double sum1, sum2, sum3, sum4;
     double temp1, temp2, temp3, temp4;

     sum1 = 0.; sum2 = 0.; sum3 = 0.; sum4 = 0.;
     temp1 = 0.; temp2 = 0.; temp3 = 0.; temp4 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN/4-1; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       sum2 += temp2;
       temp2 = *(a + 1) * *(b + 1);

       sum3 += temp3;
       temp3 = *(a + 2) * *(b + 2);

       sum4 += temp4;
       temp4 = *(a + 3) * *(b + 3);

       a += 4; b += 4;
     }

     sum1 += temp1;
     sum2 += temp2;
     sum3 += temp3;
     sum4 += temp4;

     x += sum1 + sum2 + sum3 + sum4;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("4.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   {
     int i,n; double *a=A,*b=B;
     double sum1, sum2, sum3, sum4, sum5;
     double temp1, temp2, temp3, temp4, temp5;

     sum1 = 0.; sum2 = 0.; sum3 = 0.; sum4 = 0.; sum5 = 0.;
     temp1 = 0.; temp2 = 0.; temp3 = 0.; temp4 = 0.; temp5 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN/5-1; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       sum2 += temp2;
       temp2 = *(a + 1) * *(b + 1);

       sum3 += temp3;
       temp3 = *(a + 2) * *(b + 2);

       sum4 += temp4;
       temp4 = *(a + 3) * *(b + 3);

       sum5 += temp5;
       temp5 = *(a + 4) * *(b + 4);

       a += 5; b += 5;
     }

     sum1 += temp1;
     sum2 += temp2;
     sum3 += temp3;
     sum4 += temp4;
     sum5 += temp5;

     x += sum1 + sum2 + sum3 + sum4 + sum5;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("5.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   {
     int i,n; double *a=A,*b=B;
     double sum1, sum2, sum3, sum4, sum5, sum6;
     double temp1, temp2, temp3, temp4, temp5, temp6;

     sum1 = 0.; sum2 = 0.; sum3 = 0.; sum4 = 0.; sum5 = 0.; sum6 = 0.;
     temp1 = 0.; temp2 = 0.; temp3 = 0.; temp4 = 0.; temp5 = 0.; temp6 = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (i = 0; i < MAXN/6-1; i ++) {

       sum1 += temp1;
       temp1 = *(a + 0) * *(b + 0);

       sum2 += temp2;
       temp2 = *(a + 1) * *(b + 1);

       sum3 += temp3;
       temp3 = *(a + 2) * *(b + 2);

       sum4 += temp4;
       temp4 = *(a + 3) * *(b + 3);

       sum5 += temp5;
       temp5 = *(a + 4) * *(b + 4);

       sum6 += temp6;
       temp6 = *(a + 5) * *(b + 5);

       a += 6; b += 6;
     }

     sum1 += temp1;
     sum2 += temp2;
     sum3 += temp3;
     sum4 += temp4;
     sum5 += temp5;

     x += sum1 + sum2 + sum3 + sum4 + sum5;

     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("6.\nTot cycles: %d\nL1 misses:\t%d\n",
	    values[1],values[0]);
     printf("Tot cycles/elt: %e\nL1 misses/elt:\t%e\n\n",
	    values[1]/(1.*MAXN),values[0]/(1.*MAXN));

   }

   return 0;
}

