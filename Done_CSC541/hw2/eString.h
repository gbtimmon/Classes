#ifndef _eString_
#define _eString_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char*  get_line(FILE*); 
char*  chomp(char*); 
char** split(const char*, const char*, size_t*); 
char*  strdup(const char*);

#endif
