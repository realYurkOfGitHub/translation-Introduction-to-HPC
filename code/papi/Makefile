################################################################
################################################################
####
#### This Makefile belong with the sources
#### for Introduction to High-Performance Scientific Computing
#### by Victor Eijkhout
####
#### copyright 2010-2020 Victor Eijkhout
####
#### makefile for PAPI examples
####
################################################################
################################################################

info ::
	@echo "make examples, or make any of <${EXAMPLES}>"
	@echo "  [OPTFLAGS=....]"

EXAMPLES = all_native_events tlb assoc line size inner1 inner2 thread thread2 \
    unroll
examples : ${EXAMPLES}

CC = icc
CFLAGS = ${OPTFLAGS} -I${TACC_PAPI_INC} -I. ${CDEFS} -restrict
OPTFLAGS = -O2
CDEFS = 
PTHREAD_thread = -lpthread
PTHREAD_thread2 = -lpthread
% : %.c # delete!
%.o : %.c
	${CC} -c -std=c99 ${CFLAGS} -DWAY=${WAY} $<
% : %.o 
	${CC} -o $@ $^ \
	  -Wl,-rpath,${TACC_PAPI_LIB} -L${TACC_PAPI_LIB} -lpapi ${PTHREAD_$@}
# test_utils.o

PROGRAM = tlb
ARGS = 
WAY = ${WAY_${PROGRAM}}
WAY_assoc = 1
WAY_lines = 1
WAY_size = 1
WAY_thread = 2
WAY_thread2 = 2
WAY_tlb = 1
info ::
	@echo "make run [PROGRAM=...(default=${PROGRAM})] [ARGS=... (default=${ARGS})]"
run :
	cat jobscr \
	  | sed -e s/PROGRAM/${PROGRAM}/g -e s/ARGS/${ARGS}/g \
	        -e s/WAY/${WAY}/g \
	  > jobscript
	qsub jobscript

info ::
	@echo "make process [PROGRAM=... (default=$(PROGRAM))]"
process ::
	case $(PROGRAM) in \
	  ( tlb ) \
	    ./tlbplot.py row ; cp row.pdf ../../graphics/tlb_row.pdf ; \
	    ./tlbplot.py col ; cp col.pdf ../../graphics/tlb_col.pdf ; \
	    ;; \
	  ( assoc* ) \
	    ./assocplot.py ${PROGRAM} ; \
	      cp ${PROGRAM}.pdf ../../graphics/l1_${PROGRAM}.pdf ; \
	    ;; \
	  ( line ) \
	    ./lineplot.py line ; \
	      cp line.pdf ../../graphics/cacheline16.pdf ; \
	    ./lineplot.py -n 8 line ; \
	      cp line.pdf ../../graphics/cacheline8.pdf ; \
	    ;; \
	  ( size ) \
	    ./sizeplot.py size ; \
	      cp size.pdf ../../graphics/cacheoverflow.pdf ; \
	    ;; \
	esac
clean :
	@rm -f *.o *~
	@for e in ${EXAMPLES} ; do \
	  rm -f $$e.o $$e ; done
