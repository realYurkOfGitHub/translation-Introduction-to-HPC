/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

/*
  This is a demo of a few loop optimization techniques in C
  one can use to improve numerical throughput on modern CPUs.
*/


/* 
 * Copyright (c) 2013 Jarek Siembida <jarek.siembida@gmail.com>
 * Copyright (c) 2013 LShift Ltd. <query@lshift.net>
 * 
 * PAPI instrumentation and other edits Victor Eijkhout 2014
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


// Compilation for SSE:
//
// gcc -Wall -O9 -std=c99 -march=native -mno-avx -o ve ve.c


// Compilation for AVX:
//
// gcc -Wall -O9 -std=c99 -march=native -mavx -o ve ve.c


/* Bash script for testing:
for algo in naive horizontal unrolled cached final
do
  declare perf="$(./ve "add_squares_${algo}" 200 32 3000000 2> /tmp/2 | grep performance | awk '{print $2}')"
  echo "${algo}: ${perf}"
done
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


/* // Papi setup */
/* #include "papi_test.h" */
/* #define PCHECK(e) \ */
/*   if (e!=PAPI_OK) {printf("Problem in papi call, line %d\n",__LINE__); return 1;} */
/* #define NEVENTS 2 */

typedef unsigned long long ulong_t;


#define get_usecs(t)   ((ulong_t)t.tv_sec * 1000000ULL + (ulong_t)t.tv_usec)


void add_squares_naive(float *restrict acc, float *restrict src, int len, int n) {
  float (*v)[len] = (float (*)[len])src;
  for(int i = 0; i < len; i++) {
    for(int j = 0; j < n; j++) {
      acc[i] += v[j][i] * v[j][i];
    }
  }
}


void add_squares_horizontal(float *restrict acc, float *restrict src, int len, int n) {
  float (*v)[len] = (float (*)[len])src;
  for(int j = 0; j < n; j++) {
    // Looping the inner loop "by rows" helps modern GCC use SIMD instructions
    // Older GCC versions couldn't do it anyway.
    for(int i = 0; i < len; i++) {
      acc[i] += v[j][i] * v[j][i];
    }
  }
}


void add_squares_unrolled(float *restrict acc, float *restrict src, int len, int n) {
  int i, j;
  float (*v)[len] = (float (*)[len])src;
  for(j = 0; j <= n-4; j+=4) {
    for(i = 0; i < len; i++) {
      // Unrolled loop can potentially, significantly reduce number of
      // memory reads/writes and help CPU parallelize operations across
      // multiple execution units. GCC however produces code with explicit
      // memory reads/writes for each line, this reduces gains.
      acc[i] += v[j+0][i] * v[j+0][i];
      acc[i] += v[j+1][i] * v[j+1][i];
      acc[i] += v[j+2][i] * v[j+2][i];
      acc[i] += v[j+3][i] * v[j+3][i];
    }
  }
  if (j < n) {
    add_squares_horizontal(acc, v[j], len, n - j);
  }
}


void add_squares_cached(float *restrict acc, float *restrict src, int len, int n) {
  int i, j;
  float (*v)[len] = (float (*)[len])src;
  for(j = 0; j <= n-4; j+=4) {
    for(i = 0; i < len; i++) {
      // By declaring tmp, we make GCC keep interim values in a register,
      // this reduces number of memory reads/writes and properly takes
      // advantage of loop unrolling.
      float tmp = acc[i]; 
      tmp += v[j+0][i] * v[j+0][i];
      tmp += v[j+1][i] * v[j+1][i];
      tmp += v[j+2][i] * v[j+2][i];
      tmp += v[j+3][i] * v[j+3][i];
      acc[i] = tmp;
    }
  }
  if (j < n) {
    add_squares_horizontal(acc, v[j], len, n - j);
  }
}


void add_squares_final(float *restrict acc, float *restrict src, int len, int n) {
  int i, j;
  float (*v)[len] = (float (*)[len])src;
  for(j = 0; j <= n-8; j+=8) {
    for(i = 0; i < len; i++) {
      // Multiple variables will be handled in separate registers thereby
      // creating separate dependency chains and helping CPU parallelize
      // operations across multiple execution units.
      float tmp1 = acc[i];
      float tmp2 = v[j+0][i] * v[j+0][i];
      float tmp3 = v[j+1][i] * v[j+1][i];
      tmp1      += v[j+2][i] * v[j+2][i];
      tmp2      += v[j+3][i] * v[j+3][i];
      tmp3      += v[j+4][i] * v[j+4][i];
      tmp1      += v[j+5][i] * v[j+5][i];
      tmp2      += v[j+6][i] * v[j+6][i];
      tmp3      += v[j+7][i] * v[j+7][i];
      acc[i] = tmp1 + tmp2 + tmp3;
    }
  }
  if (j < n) {
    add_squares_cached(acc, v[j], len, n - j);
  }
}

void test_fail(char *file, int line, char *call, int retval) {return;}

int main(int argc, char** argv) {
  int len, n, reps;
  void (*callback)(float *restrict, float *restrict, int, int);
  int ncallbacks = 5;
  void (**callbacks)(float *restrict, float *restrict, int, int) = 
    malloc(ncallbacks*sizeof(void (*)(float *restrict, float *restrict, int, int)));
  callbacks[0] = &add_squares_naive;
  callbacks[1] = &add_squares_horizontal;
  callbacks[2] = &add_squares_unrolled;
  callbacks[3] = &add_squares_cached;
  callbacks[4] = &add_squares_final;
  struct timeval start_time, stop_time;

  /* // PAPI stuff */
  /* int events[NEVENTS] = {PAPI_TLB_DM,PAPI_TOT_CYC}; long_long values[NEVENTS]; */
  /* int retval; */
  /* PAPI_event_info_t info, info1; */
  /* const PAPI_hw_info_t *hwinfo = NULL; */
  /* int event_code; */
  /* // const PAPI_substrate_info_t *s = NULL; */
  /* int TESTS_QUIET = 1; */
  /* // tests_quiet(argc, argv);     /\* Set TESTS_QUIET variable *\/ */

  /* retval = PAPI_library_init(PAPI_VER_CURRENT); */
  /* if (retval != PAPI_VER_CURRENT) */
  /*   test_fail(__FILE__, __LINE__, "PAPI_library_init", retval); */
  
  /* for (int i=0; i<NEVENTS; i++) { */
  /*   retval = PAPI_query_event(events[i]); PCHECK(retval); */
  /* } */

  // How much can we fit in cache?
#define L1LINES 512
#define L1LINELENGTH 8
#define L1SIZEinDOUBLES L1LINES*L1LINELENGTH

  reps = 100;
  n  = 64;
  len = (L1SIZEinDOUBLES-16)/(n+1);
  int 
    array_size = len*n*sizeof(float),
    vec_size = len*sizeof(float);
  printf("Using %d iterations of length %d, total cache %d\n\n",
	 n,len,array_size+vec_size);
  float *array, *vec;
  posix_memalign( &array, 64, array_size );
  posix_memalign( &vec, 64, vec_size );
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < len; j++) {
      array[i*len + j] = i*len + j;
    }
  }
  for(int j = 0; j < len; j++) {
    vec[j] = j;
  }

  for (int icall=0; icall<ncallbacks; icall++) {
    callback = callbacks[icall];
    gettimeofday(&start_time, NULL);
    //    retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
    for(int i = 0; i < reps; i++) {
      callback(array, vec, len, n);
    }
    //    retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
    gettimeofday(&stop_time, NULL);

    unsigned elapsed_us = (unsigned)(get_usecs(stop_time) - get_usecs(start_time));
    if (elapsed_us < 1) {
      elapsed_us = 1;
    }
    ulong_t total_fops = ((ulong_t)len) * ((ulong_t)n) * ((ulong_t)reps) * 2ULL;

    fprintf(stdout, "operations: %llu\n", total_fops);
    fprintf(stdout, "time: %u [us]\n", elapsed_us);
    fprintf(stdout, "performance: %.1f [MOPS]\n\n", ((float)total_fops)/elapsed_us);
  }

#ifdef DEBUG
  for(int i = 0; i < len; i++) {
    fprintf(stderr, "%f ", data[i]);
  }
#endif


  return 0;
}
