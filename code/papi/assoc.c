/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

/*
 * File:    assoc.c
 * Author:  Victor Eijkhout <eijkhout@tacc.utexas.edu>
 *
 * Usage: assoc m n
 */

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

#define PCHECK(e) if (e!=PAPI_OK) \
        {printf("Problem in papi call, line %d\n",__LINE__); \
         return 1;}
#define NEVENTS 2
#define MAXN 20000

/* we are assuming array storage in C row mode */
#if defined(SHIFT)
#define INDEX(i,j,m,n) (i)*(n+8)+(j)
#else
#define INDEX(i,j,m,n) (i)*(n)+(j)
#endif

int main(int argc, char **argv)
{
   int events[NEVENTS] = {PAPI_L1_DCM,PAPI_TOT_CYC}; 
   long_long values[NEVENTS];
   PAPI_event_info_t info, info1;
   const PAPI_hw_info_t *hwinfo = NULL;
   int retval,event_code, m,n, i,j;
   const PAPI_substrate_info_t *s = NULL;
   double *array;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);
   {
     int i;
     for (i=0; i<NEVENTS; i++) {
       retval = PAPI_query_event(events[i]); PCHECK(retval);
     }
   }
   /*
   if (argc<3) {
     printf("Usage: assoc m n\n"); return 1;
   } else {
     m = atoi(argv[1]); n = atoi(argv[2]);
   } printf("m,n = %d,%d\n",m,n);
   */

#if defined(SHIFT)
   array = (double*) malloc(13*(MAXN+8)*sizeof(double));
#else
   array = (double*) malloc(13*MAXN*sizeof(double));
#endif

   /* clear the array and bring in cache if possible */
   for (m=1; m<12; m++) {
     for (n=2048; n<MAXN; n=2*n) {
       printf("Run: %d,%d\n",m,n);
#if defined(SHIFT)
       printf("shifted\n");
#endif

   for (i=0; i<=m; i++)
     for (j=0; j<n; j++)
       array[INDEX(i,j,m+1,n)] = 0.;

   /* access the rows in a way to cause cache conflicts */
   retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
   for (j=0; j<n; j++)
     for (i=1; i<=m; i++)
       array[INDEX(0,j,m+1,n)] += array[INDEX(i,j,m+1,n)];
   retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
   printf("m,n=%d,%d\n#elements:\t%d\nTot cycles: %d\n",
	  m,n,m*n,values[1]);
   printf("L1 misses:\t%d\nmisses per accumulation:\t%9.5f\n\n",
	  values[0],values[0]/(1.*n));

     }
   }
   free(array);

   return 0;
}
