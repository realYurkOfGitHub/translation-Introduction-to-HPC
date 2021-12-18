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
#define MAXN (L1LINES-2)
#define ARRAYSIZE L1LINELENGTH*MAXN

struct _parms {int n,firstline,start,block,target; double *array;};
typedef struct _parms* parms;

#define INDEX(i,j) (i+j*L1LINELENGTH)

#define NCYCLES 100
void adder(void *p) {
  parms myparms = (parms)p;
  int i,j,c;
  //printf("%d %d %d\n",myparms->start,myparms->stride,myparms->block);
  for (c=0; c<NCYCLES; c++)
    for (j=myparms->firstline; j<myparms->n; j++)
      for (i=myparms->start; i<myparms->start+myparms->block; i++)
	myparms->array[INDEX(myparms->target,j)] += myparms->array[INDEX(i,j)];
  return;
}

int main(int argc, char **argv)
{
   int events[NEVENTS] = {PAPI_L1_DCM,PAPI_TOT_CYC}; long_long values[NEVENTS];
   PAPI_event_info_t info, info1;
   const PAPI_hw_info_t *hwinfo = NULL;
   int retval,event_code, m,n, xp,ithread;
   const PAPI_substrate_info_t *s = NULL;
   parms parmses[8];
   double *array;
   pthread_t threads[WAY];

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
   for (ithread=0; ithread<WAY; ithread++) {
     parmses[ithread] = (parms) malloc(sizeof(struct _parms));
   }

   for (xp=0; xp<2; xp++) {
     int n;
     if (0) {
     } else if (xp==1) {        // no conflicts
       for (ithread=0; ithread<WAY; ithread++)
	 parmses[ithread]->target = ithread;
     } else if (xp==0) { // conflicts
       for (ithread=0; ithread<WAY; ithread++)
	 parmses[ithread]->target = 0;
     }
     for (ithread=0; ithread<WAY; ithread++) {
       parmses[ithread]->firstline = WAY;
       parmses[ithread]->array = array;
       parmses[ithread]->block = L1LINELENGTH/WAY;
       parmses[ithread]->start = ithread*parmses[ithread]->block;
       parmses[ithread]->n = MAXN;
     }

     /* bring into cache */
     for (ithread=0; ithread<WAY; ithread++) {
       if (pthread_create(threads+ithread,NULL,
			  adder,(void*)(parmses+ithread))!=0) return 1;
     }
     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     for (ithread=0; ithread<WAY; ithread++) {
       if (pthread_create(threads+ithread,NULL,
			  adder,(void*)(parmses+ithread))!=0) return 1;
     }
     for (ithread=0; ithread<WAY; ithread++) {
       if (pthread_join(threads[ithread],NULL)!=0) return 2;
     }
     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("Tot cycles: %d\nL1 misses:\t%d\nCycles per op:\t%9.5f\n\n",
	    values[1],values[0],values[1]/(1.*MAXN));
   }

   free(array);

   return 0;
}
