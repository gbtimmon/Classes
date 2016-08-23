/* ***************
 * large parts of the design and architecture of this file is taken from
 * Joseph J. Pfeiffer, Jr., Ph.D. and his fuse tutorial at
 * http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/
 *
 * It has been heavily modified, but some of the code here is not my own.
 * The concepts from his project was his logging approach 
 * as well as his global state object handling. 
 * ***************/
#ifndef _GLOBAL_C
#define _GLOBAL_C

#include "global.h"
#include "file.h"

#include <fuse.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

State State_new( const char * logpath ) {

    State newState    = malloc( sizeof( struct fs_state ) ); 
    //newState->logfile = Log_open( logpath ); 
    newState->root    = File_new_root(); 

    return newState; 
}

State getState() {
    return ((State) fuse_get_context()->private_data);
}

int State_del( State s ){
    //fclose(s->logfile);
    free(s); 
    return EXIT_SUCCESS;
}

FILE* Log_open(const char * path)
{
    FILE * logfile = fopen(path, "w");
    if ( ! logfile) {
        perror("logfile");
        exit(EXIT_FAILURE);
    }

    //set logfile to line buffering
    setvbuf(logfile, NULL, _IOLBF, 0);
   
    return logfile;
}

void Log_msg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf( getState()->logfile, format, ap);
}

#endif
