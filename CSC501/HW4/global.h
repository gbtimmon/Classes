/* ***************
 * large parts of the design and architecture of this file is taken from 
 * Joseph J. Pfeiffer, Jr., Ph.D. and his fuse tutorial at
 * http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/
 *
 * It has been heavily modified, but some of the code here is not my own. 
 *  ***************/

#ifndef _PARAMS_H
#define _PARAMS_H

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE
#define FUSE_USE_VERSION 26

#include "file.h"

#include <stdio.h>

typedef struct fs_state {
    FILE *logfile;
    File root; 
} * State;

State State_new(const char *);
State getState();
int State_del( State ); 


FILE* Log_open(const char *);
void  Log_msg(const char *, ...);

unsigned int size_limit;
unsigned long long int size_limit_bytes; 
unsigned long long int current_size;

#endif
