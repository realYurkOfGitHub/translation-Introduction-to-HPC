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
#include "pthread.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

#define PCHECK(e) if (e!=PAPI_OK) {printf("Problem in papi call, line %d\n",__LINE__); return 1;}
#define NEVENTS 2
#define L1LINES 1024
#define L1LINELENGTH 8
#define L1SIZEinDOUBLES L1LINES*L1LINELENGTH
#define MAXN 2*L1SIZEinDOUBLES
#define ARRAYSIZE MAXN+2*L1LINELENGTH

struct _parms {int n,start,stride; double *array;};
typedef struct _parms* parms;

void adder(void *p) {
  parms myparms = (parms)p;
  int i,j;
  //printf("%d %d %d\n",myparms->start,myparms->stride,myparms->block);
  for (i=0,j=myparms->start; i<myparms->n; i++,j+=myparms->stride)
    myparms->array[j] += 1.;
  return;
}

int main(int argc, char **argv)
{
   int events[NEVENTS] = {PAPI_L1_DCM,PAPI_TOT_CYC}; long_long values[NEVENTS];
   PAPI_event_info_t info, info1;
   const PAPI_hw_info_t *hwinfo = NULL;
   int retval,event_code, m,n, i,j,xp;
   const PAPI_substrate_info_t *s = NULL;
   parms oneparms,twoparms;
   double *array;
   pthread_t threads[2];

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

   printf("allocating %d doubles\n",ARRAYSIZE);
   array = (double*) malloc((ARRAYSIZE)*sizeof(double));
   oneparms = (parms) malloc(sizeof(struct _parms));
   twoparms = (parms) malloc(sizeof(struct _parms));
   oneparms->array = twoparms->array = array;

   for (xp=0; xp<3; xp++) {
     int stride,n;
     if (xp==0) {        // two conflicting accesses per line
       stride = L1LINELENGTH;
       oneparms->start = 0; twoparms->start = L1LINELENGTH/2;
     } else if (xp==1) { // four conflicting accesses per line
       stride = L1LINELENGTH/2;
       oneparms->start = 0; twoparms->start = L1LINELENGTH/4;
     } else if (xp==2) { // no conflicts: only one access per line
       stride = 2*L1LINELENGTH;
       oneparms->start = 0; twoparms->start = L1LINELENGTH;
     }
     oneparms->stride = twoparms->stride = stride;
     oneparms->n = twoparms->n = (MAXN)/stride;
     printf(".. last loc %d\n",twoparms->start+twoparms->n*twoparms->stride);

     /* flush the cache */
     for (n=0; n<ARRAYSIZE; n++)
       array[n] = 1.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     if (pthread_create(threads+0,NULL,adder,(void*)oneparms)!=0) return 1;
     if (pthread_create(threads+1,NULL,adder,(void*)twoparms)!=0) return 2;
     if (pthread_join(threads[0],NULL)!=0) return 3;
     if (pthread_join(threads[1],NULL)!=0) return 4;
     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("Tot cycles: %d\nL1 misses:\t%d\nCycles per op:\t%9.5f\n\n",
	    values[1],values[0],values[1]/(1.*oneparms->n));
   }

   free(array);

   return 0;
}
