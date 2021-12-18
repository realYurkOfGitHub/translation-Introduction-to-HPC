/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

#include "papi_test.h"

/*  Variable to hold reporting status
	if TRUE, output is suppressed
	if FALSE output is sent to stdout
	initialized to FALSE
	declared here so it can be available globally
*/
int TESTS_QUIET = 0;

/*  Support routine to display header information to the screen
	from the hardware info data structure. The same code was duplicated
	in a number of tests and utilities. Seems to make sense to refactor.
	This may not be the best place for it to live, but it works for now.
 */
int papi_print_header (char *prompt, int event_flag, const PAPI_hw_info_t **hwinfo)
{
	if ((*hwinfo = PAPI_get_hardware_info()) == NULL)
		return(PAPI_ESBSTR);

	printf ("%s", prompt);
	printf("--------------------------------------------------------------------------------\n");
	printf("Vendor string and code   : %s (%d)\n", (*hwinfo)->vendor_string, (*hwinfo)->vendor);
	printf("Model string and code    : %s (%d)\n", (*hwinfo)->model_string, (*hwinfo)->model);
	printf("CPU Revision             : %f\n", (*hwinfo)->revision);
	printf("CPU Megahertz            : %f\n", (*hwinfo)->mhz);
	printf("CPU Clock Megahertz      : %d\n", (*hwinfo)->clock_mhz);
	printf("CPU's in this Node       : %d\n", (*hwinfo)->ncpu);
	printf("Nodes in this System     : %d\n", (*hwinfo)->nnodes);
	printf("Total CPU's              : %d\n", (*hwinfo)->totalcpus);
	printf("Number Hardware Counters : %d\n", PAPI_get_opt(PAPI_MAX_HWCTRS, NULL));
	printf("Max Multiplex Counters   : %d\n", PAPI_get_opt(PAPI_MAX_MPX_CTRS, NULL));
	printf("--------------------------------------------------------------------------------\n");
	if (event_flag)
		printf("The following correspond to fields in the PAPI_event_info_t structure.\n");
	printf("\n");
	return (PAPI_OK);
}


void validate_string(char *name, char *s)
{
  if ((s == NULL) || (strlen(s) == 0))
    {
      char s2[1024] = "";
      sprintf(s2,"%s was NULL or length 0",name);
      test_fail(__FILE__,__LINE__,s2,0);
    }
}

int approx_equals(double a, double b)
{
  if ((a >= b*(1.0-TOLERANCE)) && (a <= b*(1.0+TOLERANCE)))
    return 1;
  else
    {
      printf("Out of tolerance range %2.2f: %.0f vs %.0f [%.0f,%.0f]\n",TOLERANCE,a,b,b*(1.0-TOLERANCE),b*(1.0+TOLERANCE));
      return 0;
    }
}

long_long **allocate_test_space(int num_tests, int num_events)
{
   long_long **values;
   int i;

   values = (long_long **) malloc(num_tests * sizeof(long_long *));
   if (values == NULL)
      exit(1);
   memset(values, 0x0, num_tests * sizeof(long_long *));

   for (i = 0; i < num_tests; i++) {
      values[i] = (long_long *) malloc(num_events * sizeof(long_long));
      if (values[i] == NULL)
         exit(1);
      memset(values[i], 0x00, num_events * sizeof(long_long));
   }
   return (values);
}

void free_test_space(long_long ** values, int num_tests)
{
   int i;

   for (i = 0; i < num_tests; i++)
      free(values[i]);
   free(values);
}

/* Mask tells us what to select. 
	See test_utils.h for mask definitions
*/

int add_test_events(int *number, int *mask)
{
   int retval;
   int EventSet = PAPI_NULL;

   *number = 0;

   retval = PAPI_get_opt(PAPI_MAX_HWCTRS, NULL);
   if (retval < 1)
      test_fail(__FILE__, __LINE__, "PAPI_get_opt", retval);

   retval = PAPI_create_eventset(&EventSet);
   if (retval != PAPI_OK)
      test_fail(__FILE__, __LINE__, "PAPI_create_eventset", retval);

   if (*mask & MASK_L1_DCA) {
      retval = PAPI_add_event(EventSet, PAPI_L1_DCA);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_DCA is not available.\n");
         *mask = *mask ^ MASK_L1_DCA;
      }
   }

   if (*mask & MASK_L1_DCW) {
      retval = PAPI_add_event(EventSet, PAPI_L1_DCW);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_DCW is not available.\n");
         *mask = *mask ^ MASK_L1_DCW;
      }
   }

   if (*mask & MASK_L1_DCR) {
      retval = PAPI_add_event(EventSet, PAPI_L1_DCR);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_DCR is not available.\n");
         *mask = *mask ^ MASK_L1_DCR;
      }
   }

   if (*mask & MASK_L2_TCH) {
      retval = PAPI_add_event(EventSet, PAPI_L2_TCH);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L2_TCH is not available.\n");
         *mask = *mask ^ MASK_L2_TCH;
      }
   }

   if (*mask & MASK_L2_TCA) {
      retval = PAPI_add_event(EventSet, PAPI_L2_TCA);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L2_TCA is not available.\n");
         *mask = *mask ^ MASK_L2_TCA;
      }
   }

   if (*mask & MASK_L2_TCM) {
      retval = PAPI_add_event(EventSet, PAPI_L2_TCM);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L2_TCM is not available.\n");
         *mask = *mask ^ MASK_L2_TCM;
      }
   }

   if (*mask & MASK_L1_DCM) {
      retval = PAPI_add_event(EventSet, PAPI_L1_DCM);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_DCM is not available.\n");
         *mask = *mask ^ MASK_L1_DCM;
      }
   }

   if (*mask & MASK_L1_ICM) {
      retval = PAPI_add_event(EventSet, PAPI_L1_ICM);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_ICM is not available.\n");
         *mask = *mask ^ MASK_L1_ICM;
      }
   }

   if (*mask & MASK_L1_TCM) {
      retval = PAPI_add_event(EventSet, PAPI_L1_TCM);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_L1_TCM is not available.\n");
         *mask = *mask ^ MASK_L1_TCM;
      }
   }

   if (*mask & MASK_BR_CN) {
      retval = PAPI_add_event(EventSet, PAPI_BR_CN);
      if (retval == PAPI_OK)
         (*number)++;
      else {

         char errstring[PAPI_MAX_STR_LEN];
         PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
         if (!TESTS_QUIET) {
            fprintf(stdout,"Error: %s\n", errstring);
            fprintf(stdout, "PAPI_BR_CN is not available.\n");
         }
         *mask = *mask ^ MASK_BR_CN;
      }
   }
   if (*mask & MASK_BR_MSP) {
      retval = PAPI_add_event(EventSet, PAPI_BR_MSP);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_BR_MSP is not available.\n");
         *mask = *mask ^ MASK_BR_MSP;
      }
   }
   if (*mask & MASK_BR_PRC) {
      retval = PAPI_add_event(EventSet, PAPI_BR_PRC);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_BR_PRC is not available.\n");
         *mask = *mask ^ MASK_BR_PRC;
      }
   }

   if (*mask & MASK_FP_OPS) {
      retval = PAPI_add_event(EventSet, PAPI_FP_OPS);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_FP_OPS is not available.\n");
         *mask = *mask ^ MASK_FP_OPS;
      }
   }

   if (*mask & MASK_FP_INS) {
      retval = PAPI_add_event(EventSet, PAPI_FP_INS);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_FP_INS is not available.\n");
         *mask = *mask ^ MASK_FP_INS;
      }
   }

   if (*mask & MASK_TOT_INS) {
      retval = PAPI_add_event(EventSet, PAPI_TOT_INS);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_TOT_INS is not available.\n");
         *mask = *mask ^ MASK_TOT_INS;
      }
   }

   if (*mask & MASK_TOT_IIS) {
      retval = PAPI_add_event(EventSet, PAPI_TOT_IIS);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_TOT_IIS is not available.\n");
         *mask = *mask ^ MASK_TOT_IIS;
      }
   }

   if (*mask & MASK_TOT_CYC) {
      retval = PAPI_add_event(EventSet, PAPI_TOT_CYC);
      if (retval == PAPI_OK)
         (*number)++;
      else {
         if (!TESTS_QUIET)
            fprintf(stdout, "PAPI_TOT_CYC is not available.\n");
         *mask = *mask ^ MASK_TOT_CYC;
      }
   }

   return (EventSet);
}

int remove_test_events(int *EventSet, int mask)
{
   int retval = PAPI_OK;

   if (mask & MASK_L1_DCA) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_DCA);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L1_DCW) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_DCW);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L1_DCR) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_DCR);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L2_TCH) {
      retval = PAPI_remove_event(*EventSet, PAPI_L2_TCH);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L2_TCA) {
      retval = PAPI_remove_event(*EventSet, PAPI_L2_TCA);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L2_TCM) {
      retval = PAPI_remove_event(*EventSet, PAPI_L2_TCM);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L1_DCM) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_DCM);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L1_ICM) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_ICM);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_L1_TCM) {
      retval = PAPI_remove_event(*EventSet, PAPI_L1_TCM);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_FP_OPS) {
      retval = PAPI_remove_event(*EventSet, PAPI_FP_OPS);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_FP_INS) {
      retval = PAPI_remove_event(*EventSet, PAPI_FP_INS);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_TOT_INS) {
      retval = PAPI_remove_event(*EventSet, PAPI_TOT_INS);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_TOT_IIS) {
      retval = PAPI_remove_event(*EventSet, PAPI_TOT_IIS);
      if (retval < PAPI_OK)
         return (retval);
   }

   if (mask & MASK_TOT_CYC) {
      retval = PAPI_remove_event(*EventSet, PAPI_TOT_CYC);
      if (retval < PAPI_OK)
         return (retval);
   }

   return (PAPI_destroy_eventset(EventSet));
}

char *stringify_all_domains(int domains)
{
  static char buf[PAPI_HUGE_STR_LEN];
  int i, did = 0;
  buf[0] = '\0';

  for (i=PAPI_DOM_MIN;i<=PAPI_DOM_MAX;i=i<<1)
    if (domains&i)
      {
	if (did)
	  strcpy(buf+strlen(buf),"|");
	strcpy(buf+strlen(buf),stringify_domain(domains&i));
	did++;
      }
  if (did == 0)
     test_fail(__FILE__, __LINE__, "Unrecognized domains!", 0);    
  return(buf);
}

char *stringify_domain(int domain)
{
  switch (domain) {
   case PAPI_DOM_SUPERVISOR:
      return ("PAPI_DOM_SUPERVISOR");
   case PAPI_DOM_USER:
      return ("PAPI_DOM_USER");
   case PAPI_DOM_KERNEL:
      return ("PAPI_DOM_KERNEL");
   case PAPI_DOM_OTHER:
      return ("PAPI_DOM_OTHER");
   case PAPI_DOM_ALL:
      return ("PAPI_DOM_ALL");
   default:
     test_fail(__FILE__, __LINE__, "Unrecognized domains!", 0);
   }
   return (NULL);
}

char *stringify_all_granularities(int granularities)
{
  static char buf[PAPI_HUGE_STR_LEN];
  int i, did = 0;

  buf[0] = '\0';
  for (i=PAPI_GRN_MIN;i<=PAPI_GRN_MAX;i=i<<1)
    if (granularities&i)
      {
	if (did)
	  strcpy(buf+strlen(buf),"|");
	strcpy(buf+strlen(buf),stringify_granularity(granularities&i));
	did++;
      }
  if (did == 0)
     test_fail(__FILE__, __LINE__, "Unrecognized granularity!", 0);    
    
  return(buf);
}

char *stringify_granularity(int granularity)
{
   switch (granularity) {
   case PAPI_GRN_THR:
      return ("PAPI_GRN_THR");
   case PAPI_GRN_PROC:
      return ("PAPI_GRN_PROC");
   case PAPI_GRN_PROCG:
      return ("PAPI_GRN_PROCG");
   case PAPI_GRN_SYS_CPU:
      return ("PAPI_GRN_SYS_CPU");
   case PAPI_GRN_SYS:
      return ("PAPI_GRN_SYS");
   default:
     test_fail(__FILE__, __LINE__, "Unrecognized granularity!", 0);
   }
   return (NULL);
}

void tests_quiet(int argc, char **argv)
{
  /* if ((argc > 1) && (strcasecmp(argv[1], "TESTS_QUIET") == 0)) */
  /*   { */
  /*     TESTS_QUIET = 1; */
  /*   } */
  /* else */
    {
      int retval;
      
      retval = PAPI_set_debug(PAPI_VERB_ECONT);
      if (retval != PAPI_OK)
	test_fail(__FILE__, __LINE__, "PAPI_set_debug", retval);
    }
}

void test_pass(char *file, long_long ** values, int num_tests)
{
   fprintf(stdout,"%-40s PASSED\n", file);
   if (values)
      free_test_space(values, num_tests);
   if ( PAPI_is_initialized() ) PAPI_shutdown();
   exit(0);
}

void test_fail(char *file, int line, char *call, int retval)
{
   char buf[128];

   memset(buf, '\0', sizeof(buf));
   fprintf(stdout,"%-40s FAILED\nLine # %d\n", file, line);
   if (retval == PAPI_ESYS) {
      sprintf(buf, "System error in %s", call);
      perror(buf);
   } else if (retval > 0) {
      fprintf(stdout,"Error: %s\n", call);
   } else if (retval == 0) {
#if defined(sgi)
      fprintf(stdout,"SGI requires root permissions for this test\n");
#else
      fprintf(stdout,"Error: %s\n", call);
#endif
   } else {
      char errstring[PAPI_MAX_STR_LEN];
      PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
      fprintf(stdout,"Error in %s: %s\n", call, errstring);
   }
   fprintf(stdout,"\n");
   if ( PAPI_is_initialized() ) PAPI_shutdown();
   exit(1);
}

void test_skip(char *file, int line, char *call, int retval)
{
   char buf[128];

   memset(buf, '\0', sizeof(buf));
   fprintf(stdout,"%-40s SKIPPED\n", file);
   if (!TESTS_QUIET) {
      if (retval == PAPI_ESYS) {
         fprintf(stdout,"Line # %d\n", line);
         sprintf(buf, "System error in %s:", call);
         perror(buf);
      } else if ( retval == PAPI_EPERM ){
         fprintf(stdout,"Line # %d\n", line);
         fprintf(stdout, "Invalid permissions for %s.",call);
      } else if (retval >= 0) {
         fprintf(stdout,"Line # %d\n", line);
         fprintf(stdout,"Error calculating: %s\n", call);
      } else if (retval < 0) {
         char errstring[PAPI_MAX_STR_LEN];
         fprintf(stdout,"Line # %d\n", line);
         PAPI_perror(retval, errstring, PAPI_MAX_STR_LEN);
         fprintf(stdout,"Error in %s: %s\n", call, errstring);
      }
      fprintf(stdout,"\n");
   }
   if ( PAPI_is_initialized() ) PAPI_shutdown();
   exit(0);
}

#ifdef _WIN32
#undef exit
void wait(char *prompt)
{
   HANDLE hStdIn;
   BOOL bSuccess;
   INPUT_RECORD inputBuffer;
   DWORD dwInputEvents;         /* number of events actually read */

   printf(prompt);
   hStdIn = GetStdHandle(STD_INPUT_HANDLE);
   do {
      bSuccess = ReadConsoleInput(hStdIn, &inputBuffer, 1, &dwInputEvents);
   } while (!(inputBuffer.EventType == KEY_EVENT && inputBuffer.Event.KeyEvent.bKeyDown));
}

int wait_exit(int retval)
{
   if (!TESTS_QUIET)
      wait("Press any key to continue...\n");
   exit(retval);
}

#define exit wait_exit
#endif

void test_print_event_header(char *call, int evset)
{
   int ev_ids[PAPI_MAX_HWCTRS + PAPI_MPX_DEF_DEG];
   int i, nev;
   int retval;
   char evname[PAPI_MAX_STR_LEN];

   nev = PAPI_MAX_HWCTRS + PAPI_MPX_DEF_DEG;
   retval = PAPI_list_events(evset, ev_ids, &nev);

   if (*call)
      fprintf(stdout,"%s", call);
   if (retval == PAPI_OK) {
      for (i = 0; i < nev; i++) {
         PAPI_event_code_to_name(ev_ids[i], evname);
         printf(ONEHDR, evname);
      }
   } else {
      fprintf(stdout,"Can not list event names.");
   }
   fprintf(stdout,"\n");
}

int add_two_events(int *num_events, int *papi_event, 
               const PAPI_hw_info_t *hw_info, int *mask)
{
  /* query and set up the right event to monitor */
   int EventSet = PAPI_NULL;
  PAPI_event_info_t info;
  unsigned int potential_evt_to_add[3][2] = {{ PAPI_FP_INS, MASK_FP_INS},{ PAPI_FP_OPS, MASK_FP_OPS}, { PAPI_TOT_INS, MASK_TOT_INS}};
  int i = 0;
  unsigned int counters, event_found = 0;
  
  *mask = 0;
  counters = (unsigned int)PAPI_num_hwctrs();
  while ((i < 3) && (!event_found)) {
    if (PAPI_query_event(potential_evt_to_add[i][0]) == PAPI_OK) {
      if (PAPI_get_event_info(potential_evt_to_add[i][0], &info) == PAPI_OK) {
#if defined(__crayx2)					/* CRAY X2 */
	if ((info.count > 0) && (counters > info.count) && !strcmp(info.derived,"NOT_DERIVED")) event_found = 1;
#else
	if ((info.count > 0) && (counters > info.count)) event_found = 1;
#endif
      }
    }
    if (!event_found)
      i++;
  }
  if (event_found) {
    *papi_event = potential_evt_to_add[i][0];
    *mask =  potential_evt_to_add[i][1] | MASK_TOT_CYC;
   EventSet = add_test_events(num_events, mask);
  } else {
    test_fail(__FILE__, __LINE__, "Not enough room to add an event!", 0);
  }
   return(EventSet);
}

int add_two_nonderived_events(int *num_events, int *papi_event, 
               const PAPI_hw_info_t *hw_info, int *mask)
{
  /* query and set up the right event to monitor */
   int EventSet = PAPI_NULL;
  PAPI_event_info_t info;
  unsigned int potential_evt_to_add[3][2] = {{ PAPI_FP_INS, MASK_FP_INS},{ PAPI_FP_OPS, MASK_FP_OPS}, { PAPI_TOT_INS, MASK_TOT_INS}};
  int i = 0;
  unsigned int counters, event_found = 0;
  
  *mask = 0;
  counters = (unsigned int)PAPI_num_hwctrs();
  while ((i < 3) && (!event_found)) {
    if (PAPI_query_event(potential_evt_to_add[i][0]) == PAPI_OK) {
      if (PAPI_get_event_info(potential_evt_to_add[i][0], &info) == PAPI_OK) {
	    if ((info.count > 0) && (counters > info.count) && !strcmp(info.derived,"NOT_DERIVED")) 
          event_found = 1;
      }
    }
    if (!event_found)
      i++;
  }
  if (event_found) {
    *papi_event = potential_evt_to_add[i][0];
    *mask =  potential_evt_to_add[i][1] | MASK_TOT_CYC;
   EventSet = add_test_events(num_events, mask);
  } else {
    test_fail(__FILE__, __LINE__, "Not enough room to add an event!", 0);
  }
   return(EventSet);
}

/* add native events to use all counters */
int enum_add_native_events(int *num_events, int **evtcodes)
{
	/* query and set up the right event to monitor */
	int EventSet = PAPI_NULL;
	int i = 0, k, event_code, retval;
	unsigned int counters, event_found = 0;
	PAPI_event_info_t info;

	// This is version 3 stuff.
	/* const PAPI_substrate_info_t *s = NULL; */
	/* s = PAPI_get_substrate_info(); */

	counters = (unsigned int)PAPI_num_hwctrs();
	(*evtcodes) = (int *)calloc(counters, sizeof(int));

	retval = PAPI_create_eventset(&EventSet);
	if (retval != PAPI_OK)
		test_fail(__FILE__, __LINE__, "PAPI_create_eventset", retval);

	/* For platform independence, always ASK FOR the first event */
	/* Don't just assume it'll be the first numeric value */
	i = 0 | PAPI_NATIVE_MASK;
	PAPI_enum_event(&i, PAPI_ENUM_FIRST);

	do {
		retval = PAPI_get_event_info(i, &info);

		if (0 /* s->cntr_umasks */ ) {
			k = i;
			if (PAPI_enum_event(&k, PAPI_NTV_ENUM_UMASKS) == PAPI_OK) {
				do {
					retval = PAPI_get_event_info(k, &info);
					event_code = info.event_code;
					retval = PAPI_add_event(EventSet, event_code);
					if (retval == PAPI_OK){
						(*evtcodes)[event_found] = event_code;
						event_found ++;
					}
					else {
						if (!TESTS_QUIET)
							fprintf(stdout, "%d is not available.\n", event_code);
					}
				} while (PAPI_enum_event(&k, PAPI_NTV_ENUM_UMASKS) == PAPI_OK && event_found<counters);
			} else {
				event_code = info.event_code;
				retval = PAPI_add_event(EventSet, event_code);
				if (retval == PAPI_OK){
					(*evtcodes)[event_found] = event_code;
					event_found ++;
				}
			}
			if (!TESTS_QUIET && retval == PAPI_OK)
				printf("\n");
		} else {
			event_code = info.event_code;
			retval = PAPI_add_event(EventSet, event_code);
			if (retval == PAPI_OK){
				(*evtcodes)[event_found] = event_code;
				event_found ++;
			}
			else {
				if (!TESTS_QUIET)
					fprintf(stdout, "%d is not available.\n", event_code);
			}
		}
	} while (PAPI_enum_event(&i, PAPI_ENUM_EVENTS) == PAPI_OK && event_found<counters);

	*num_events = event_found;
	return(EventSet);
}
