#ifndef USH_C
#define USH_C

#define _GNU_SOURCE

#include "ush.h"
#include "builtins.h"
#include "parse.h"
#include "job.h"
#include "util.h"

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define OK           0
#define NOT_OK       1

#define PROMPT()     printf("%s%%", hostname)
#define RC_FILE      "/.ushrc"

#define IS_PARENT(x) ( x >  0 )
#define IS_CHILD(x)  ( x == 0 )

#define NEW_PIPE()   malloc(sizeof(int) * 2)
#define PIPE_RD(x)   x[0]
#define PIPE_WT(x)   x[1]

#define BUFFER_SIZE  512
#define LOG(x)       dprintf( STDERR_FD, "[LOG] :  %s\n", x);

static int STDIN_FD; 
static int STDOUT_FD; 
static int STDERR_FD; 

static char hostname[512]; 

const char* getHome() {
    return (getenv("HOME") == NULL)
        ? getpwuid( getuid() )->pw_dir
        : getenv("HOME");
}

const char* getRCPath() { 

    char f[BUFFER_SIZE];
    strcpy(f, getHome());
    strcat(f, RC_FILE);
    return (const char*) f;
}

int main(int argc, char** args, char** env){

    STDIN_FD  = dup( STDIN_FILENO  );
    STDOUT_FD = dup( STDOUT_FILENO ); 
    STDERR_FD = dup( STDERR_FILENO ); 
    
       
    gethostname(hostname, BUFFER_SIZE);

    
    
    int ushrc_fd = open( 

    while( PROMPT() ) { 
        Pipe p = parse();
     
        if( !p ) continue; 
        Job j = Job_build(p);
        Job_run(j); 
        Job_print(j, "PreJoin");
        Job_join(j);
        Job_free(j); 
    }      
}



#endif
