#ifndef MASTER_C
#define MASTER_C

#include "socket.h"
#include "connection.h"
#include "potato.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main ( int argc, char** argv, char** envv) { 
     
    if( argc < 4 ) {
        fprintf(stderr, "%s: <port> <childcount> <swapcount>\n", argv[0]);
        exit(1); 
    }

}
#endif
