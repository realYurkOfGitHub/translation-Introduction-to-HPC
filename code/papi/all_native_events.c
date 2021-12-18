/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

/*
 * File:    all_native_events.c
 * Author:  Haihang You
 *	        you@cs.utk.edu
 * Mods:    <Your name here>
 *          <Your email here>
 */

/* This file hardware info and performs the following test:
     Attempts to add all available native events to an event set.
     This is a good preliminary way to validate native event tables.
*/

#include "papi_test.h"
extern int TESTS_QUIET;         /* Declared in test_utils.c */

static int add_remove_event(int EventSet, int event_code, char *name) {
    int retval;
    char errstring[PAPI_MAX_STR_LEN];
    long_long values;

    retval = PAPI_add_event(EventSet, event_code);
    if (retval != PAPI_OK) {
	  printf("Error adding %s\n", name);
	  return(0);
    } 
    else 
      printf("Added %s successfully.\n", name);

    retval = PAPI_start(EventSet);
    if (retval != PAPI_OK) {
      PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
      fprintf(stdout,"Error Starting %s : %s\n", name, errstring);
    } 
    else { 
      retval = PAPI_stop(EventSet, &values);
      if (retval != PAPI_OK) {
        PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
		fprintf(stdout,"Error Stopping %s: %s\n", name, errstring);
	    return(0);
      }
      else {
        printf("Stopped %s successfully.\n", name);
      }
	}

    retval = PAPI_remove_event(EventSet, event_code);
    if (retval != PAPI_OK){
      PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
	  printf("Error removing %s: %s\n", name, errstring);
	  return(0);
    }
    else printf("Removed %s successfully.\n", name);
    return(1);
}

int main(int argc, char **argv)
{
   int i, k, EventSet=PAPI_NULL, add_count=0, err_count=0;
   int retval;
   PAPI_event_info_t info, info1;
   const PAPI_hw_info_t *hwinfo = NULL;
   int event_code;
   const PAPI_substrate_info_t *s = NULL;

   tests_quiet(argc, argv);     /* Set TESTS_QUIET variable */
   /*for(i=0;i<argc;i++) */

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT)
      test_fail(__FILE__, __LINE__, "PAPI_library_init", retval);

   retval = PAPI_create_eventset(&EventSet);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_create_eventset", retval);

   retval = papi_print_header
	  ("Test case ALL_NATIVE_EVENTS: Available native events and hardware information.\n", 0, &hwinfo);
   if (retval != PAPI_OK) test_fail(__FILE__, __LINE__, "PAPI_get_hardware_info", 2);

   if ((s = PAPI_get_substrate_info()) == NULL)
      test_fail(__FILE__, __LINE__, "PAPI_get_substrate_info", 2);

   /* For platform independence, always ASK FOR the first event */
   /* Don't just assume it'll be the first numeric value */
   i = 0 | PAPI_NATIVE_MASK;
   PAPI_enum_event(&i, PAPI_ENUM_FIRST);

   do {
	retval = PAPI_get_event_info(i, &info);
	if (s->cntr_umasks) {
		k = i;
		if (PAPI_enum_event(&k, PAPI_NTV_ENUM_UMASKS) == PAPI_OK) {
			do {
				retval = PAPI_get_event_info(k, &info1);
				event_code = info1.event_code;
				if (add_remove_event(EventSet, event_code, info1.symbol))
					add_count++;
				else err_count++;
			} while (PAPI_enum_event(&k, PAPI_NTV_ENUM_UMASKS) == PAPI_OK);
		}
		else{
		  event_code = info.event_code;
  		  if (add_remove_event(EventSet, event_code, info.symbol))
			add_count++;
		  else err_count++;
	       }
	}
	else {
		event_code = info.event_code;
		if (s->cntr_groups) event_code &= ~PAPI_NTV_GROUP_AND_MASK;
		if (add_remove_event(EventSet, event_code, info.symbol))
			add_count++;
		else err_count++;
	}
   } while (PAPI_enum_event(&i, PAPI_ENUM_EVENTS) == PAPI_OK);

    printf("\n\nSuccessfully found, added, and removed %d events.\n", add_count);
    if (err_count)
    printf("Failed to add %d events.\n", err_count);
    if ( add_count > 0 )
      test_pass(__FILE__, NULL, 0);
    else
      test_fail(__FILE__, __LINE__, "No events added", 1);
    exit(1);
}
