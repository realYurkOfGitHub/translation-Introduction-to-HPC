# -*_ makefile -*-
################################################################
################################################################
####
#### This Makefile belong with the sources
#### for Introduction to High-Performance Scientific Computing
#### by Victor Eijkhout
#### copyright 2010-2021
####
#### makefile for make exercise
####
################################################################
################################################################

info ::
	@echo
	@echo "================================"
	@echo "makefile for code/make" && echo
	@echo "The solution makefile for this exercise"
	@echo " is in Makefile.lib"

CC = clang -std=c99

info ::
	@echo
	@echo "make package : tar the source and header files into <<${TARFILE}>>"
.PHONY: package
TARFILE = source_and_lib.tgz
package :
	@echo "Creating: ${TARFILE}"
	tar fczv ${TARFILE} *.cxx *.h

include ../../makefiles/Make.clean
clean ::
	@rm -f prog main
info ::
	@echo "make total_clean : remove tgz files too"
total_clean ::
	@rm -f *.tgz

info ::
	@echo "--------------------------------" && echo

