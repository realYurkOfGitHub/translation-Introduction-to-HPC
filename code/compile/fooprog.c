/****************************************************************
 ****
 **** This program source is part of
 **** Introduction to High-performance Scientific Computing
 **** by Victor Eijkhout
 **** copyright Victor Eijkhout 2011-2021
 ****
 **** fooprog.c : simple main program using external function
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>

//codesnippet foomain
extern void bar(char*);

int main() {
  bar("hello world\n");
  return 0;
}
//codesnippet end
