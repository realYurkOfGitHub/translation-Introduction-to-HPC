/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

/*
 * File:    size.c
 * Author:  Victor Eijkhout <eijkhout@tacc.utexas.edu>
 *
 * Usage: size
 */

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

#define PCHECK(e) \
  if (e!=PAPI_OK) \
    {printf("Problem in papi call, line %d\n",__LINE__); return 1;}
#define NEVENTS 3
#define NRUNS 200
#define L1WORDS 8096
#define L2WORDS 100000

int main(int argc, char **argv)
{
  int events[NEVENTS] =
    {
      PAPI_TOT_CYC,/* total cycles */
      PAPI_L1_DCM, /* stalls on L1 cache miss */
      PAPI_L2_DCM, /* stalls on L2 cache miss */
    };
  long_long values[NEVENTS];
  PAPI_event_info_t info, info1;
  const PAPI_hw_info_t *hwinfo = NULL;
  int retval,event_code, m,n, i,j,size, arraysize;
  const PAPI_substrate_info_t *s = NULL;
  double *array;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);
   {
     int i;
     for (i=0; i<NEVENTS; i++) {
       retval = PAPI_query_event(events[i]) ; PCHECK(retval);
     }
   }

   /* declare an array that is more than twice the L2 cache size */
   arraysize=2*L2WORDS;
   array = (double*) malloc(arraysize*sizeof(double));

   for (size=L1WORDS/4; size<arraysize; size+=L1WORDS/4) {
     printf("Run: data set size=%d\n",size);

     /* clear the cache by dragging the whole array through it */
     for (n=0; n<arraysize; n++) array[n] = 0.;
     /* now load the data in the highest cache level that fits */
     for (n=0; n<size; n++) array[n] = 0.;

     retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
     /* run the experiment */
     for (i=0; i<NRUNS; i++) {
       for (j=0; j<size; j++) array[j] = 2.3*array[j]+1.2; 
     }
     retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
     printf("size=%d\nTot cycles: %d\n",size,values[0]);
     printf("cycles per array loc: %9.5f\n",size,values[0]/(1.*NRUNS*size));
     printf("L1 misses:\t%d\nfraction of L1 lines missed:\t%9.5f\n",
	    values[1],values[1]/(size/8.));
     printf("L2 misses:\t%d\nfraction of L2 lines missed:\t%9.5f\n",
	    values[2],values[2]/(size/8.));
     printf("\n");
   }
   free(array);

   return 0;
}
