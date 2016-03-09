#ifndef MASTER_C
#define MASTER_C

#define _GNU_SOURCE

#include <stdio.h> 
#include <unistd.h>

#define BUFFER 256

static char hostname[BUFFER];

int main(int argc, char** argp, char** envp){

    gethostname(hostname, BUFFER);

    printf("Hello World from %s\n", hostname);

}

#endif
