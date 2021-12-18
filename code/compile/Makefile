# -*- makefile -*-
################################################################
################################################################
####
#### This Makefile belong with the sources
#### for Introduction to High-Performance Scientific Computing
#### by Victor Eijkhout
#### copyright 2010-2021
####
#### makefile for code/compile
####
################################################################
################################################################

info ::

include ../../makefiles/Make.compile

PROGRAMS = 

info ::
	@echo "make PROGRAM (where PROGRAM in ${PROGRAMS})"
	@echo
PROGRAMS += oneprogram
PROGRAM = oneprogram
OBJS = 
oneprogram : fooprog.o foosub.o
	${CLINKER} -g -O2 -o $@ $^

info ::
	@echo "%%%% illustration of compilation and linking"
	@echo "make run_makeoneprogram run_makeseparatecompile"
	@echo
.PHONY: run_makeoneprogram run_makeseparatecompile
run_makeoneprogram : example_clean
	${CLINKER} -o oneprogram fooprog.c foosub.c
	./oneprogram
.PHONY: run_makeseparatecompile
run_makeseparatecompile : example_clean
	@make --no-print-directory oneprogram
	./oneprogram

####
#### use of libraries
####
info ::
	@echo "%%%% use of libraries"
	@echo "make staticprogram"
PROGRAMS += staticprogram
LIBS += libs/libfoo.a
staticprogram : fooprog.o makestaticlib
	${CLINKER}  -o $@ $< -Llibs -lfoo
	@ls -l $@
libs/libfoo.a : foosub.o
	@mkdir -p libs
	@rm -f $@
	for o in $^ ; do \
	  ar cr $@ $${o} ; \
	done
.PHONY: makestaticlib
PRINT = 1
makestaticlib : example_clean
	@if [ ${PRINT} = "1" ] ; then \
	    echo && echo "==== Making static library ====" && echo ; fi
	@make --no-print-directory libs/libfoo.a
info ::
	@echo "make dynamicprogram"
PROGRAMS += dynamicprogram
dynamicprogram : fooprog.o makedynamiclib
	${CLINKER} -o $@ fooprog.o -Llibs -lfoo
	@ls -l $@
info ::
	@echo "make rpathprogram"
PROGRAMS += rpathprogram
rpathprogram : fooprog.o  makedynamiclib
	${CLINKER} -o $@ fooprog.o -Wl,-rpath=./libs -Llibs -lfoo
	@ls -l $@
SHAREOPTION = -shared
.PHONY: makedynamiclib run_makedynamiclib
makedynamiclib : example_clean 
	@make --no-print-directory PIC_OPTION=-fPIC libs/libfoo.so
run_makedynamiclib : 
	@echo "%%%%"; echo "Demonstration: making shared library" ; echo "%%%%"
	@make --no-print-directory makedynamiclib

LIBS += libs/libfoo.so
libs/libfoo.so : foosub.o
	@mkdir -p libs
	@rm -f $@
	${CLINKER} -o $@ ${SHAREOPTION} $^
info ::
	@echo

info ::
	@echo "%%%% illustration of static libraries"
	@echo "make run_staticprogram"
	@echo
.PHONY: run_staticprogram
run_staticprogram : example_clean 
	@echo ; echo "==== Use of static library ====" ; echo
	@make --no-print-directory staticprogram PRINT=0
	@echo ; echo ".. running:" ; echo
	@./staticprogram

info ::
	@echo "%%%% illustration of dynamic libraries"
	@echo "make run_dynamicprogram run_rpathprogram"
	@echo 
.PHONY: run_dynamicprogram
run_dynamicprogram : example_clean
	@echo ; echo "Linking to shared library" ; echo
	@make --no-print-directory dynamicprogram
	@echo ; echo ".. note the size of the program" ; echo
	@ls -l dynamicprogram
	@echo ; echo ".. note unresolved link to a library" ; echo
	@cmd="ldd dynamicprogram | grep libfoo" \
	 && if [ `uname` = "Darwin" ] ; then \
	        cmd="otool -L dynamicprogram | grep libfoo" ; fi \
	 && echo $$cmd \
	 && eval $$cmd
	@echo ; echo ".. running by itself:" ; echo
	@make --no-print-directory runshared_wrong
	@echo ; echo ".. note resolved with LD_LIBRARY_PATH" ; echo
	@cmd="LD_LIBRARY_PATH=\$${LD_LIBRARY_PATH}:./libs ldd dynamicprogram | grep libfoo" \
	 && if [ `uname` = "Darwin" ] ; then \
	        cmd="LD_LIBRARY_PATH=\$${LD_LIBRARY_PATH}:./libs otool -L dynamicprogram | grep libfoo" \
	    ; fi \
	 && echo $$cmd \
	 && eval $$cmd
	@echo ; echo ".. running with updated library path:" ; echo
	@make --no-print-directory runshared_right
.PHONY: runshared_wrong runshared_right
runshared_wrong : dynamicprogram
	@./dynamicprogram || /bin/true
runshared_right : dynamicprogram
	@cmd="LD_LIBRARY_PATH=\$${LD_LIBRARY_PATH}:./libs dynamicprogram" \
	 && echo $$cmd \
	 && eval $$cmd
.PHONY: run_rpathprogram
run_rpathprogram : rpathprogram
	./rpathprogram


info ::
	@echo "%%%% study of optimization levels"
	@echo "make optimize PROGRAM=... (from: ${ROTATE_PROGRAMS})"
	@echo
ROTATE_PROGRAMS = rotate rotate_common
.PHONY: optimize
gcc_OPT_REPORT = -fopt-info
# -fprofile-report
intel_OPT_REPORT = -qopt-report=4
OPT_REPORT = ${${TACC_FAMILY_COMPILER}_OPT_REPORT}
optimize :
	@program=${PROGRAM} \
	 && for opt in 0 1 2 3 ; do \
	        echo "Compiling <<with optimization level $$opt" \
	        && ${C_COMPILER} -std=c99 \
	            -O$${opt} ${OPT__REPORT} \
	            -o $$program $$program.c -lm \
	        && ./$$program \
	        ; done

##
## code stripping
##
SOURCES = fooprog.c foosub.c rotate.c hello.c
info ::
	@echo "make strip_sources : remove header"
.PHONY: strip_sources
strip_sources :
	for s in ${SOURCES} ; do \
	    cat $$s | awk 'p==1 && !/snippet/ {print} NF==0 {p=1}' > $$s.stripped.out \
	    ; done
total_clean ::
	@rm -f *.stripped_out

##
## clean rules
##
include ../../makefiles/Make.clean
clean ::
	@rm -f *.optrpt *.annot

.PHONY: example_clean
info ::
	@echo "make example_clean : remove local binaries"
example_clean ::
	@rm -f ${PROGRAMS} ${LIBS}
clean :: example_clean
	@rm -rf libs
