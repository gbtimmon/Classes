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
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define OK           0
#define NOT_OK       1

#define PROMPT()     printf("%s%%", hostname)
#define RC_FILE      "/.ushrc"

#define IS_PARENT(x) ( x >  0 )
#define IS_CHILD(x)  ( x == 0 )

#define NEW_PIPE()   malloc(sizeof(int) * 2)
#define PIPE_RD(x)   x[0]
#define PIPE_WT(x)   x[1]

#define IS_END(x)    !strcmp(j->head->cmd->args[0], "end")
#define CHUNK_SIZE   32 
#define BUFFER_SIZE  256
#define LOG(x)       dprintf( STDERR_FD, "[LOG] :  %s\n", x);

static int STDIN_FD; 
static int STDOUT_FD; 
static int STDERR_FD; 

static char hostname[BUFFER_SIZE]        = "" ; 
static char rc_path[BUFFER_SIZE]         = "";

const char* getHome() {
    return (getenv("HOME") == NULL)
        ? getpwuid( getuid() )->pw_dir
        : getenv("HOME");
}

char* getRCPath() { 
    if(rc_path[0] == '\0') {
        strcpy(rc_path, getHome());
        strcat(rc_path, RC_FILE);
    }
    return rc_path;
}

int batch() {
        Job j = Job_build(parse());
        Job_run(j);
        Job_join(j);
        Job_free(j);
        return OK;
}

int main(int argc, char** args, char** env){

    STDIN_FD  = dup( STDIN_FILENO  );
    STDOUT_FD = dup( STDOUT_FILENO ); 
    STDERR_FD = dup( STDERR_FILENO ); 
    
       
    gethostname(hostname, BUFFER_SIZE);

    int rc_fd = open( getRCPath(), O_RDONLY ); 

    if( rc_fd < 0 ) {
        dprintf( STDERR_FD, "Failed to open config %s. Errno %d [%s]\n",  getRCPath(), errno, strerror(errno));
    }else {
        close(STDIN_FILENO);
        dup2( rc_fd, STDIN_FILENO); 

        batch();

        close(STDIN_FILENO);
        dup2( dup(STDIN_FD), STDIN_FILENO);
    }
   
    

    while( PROMPT() ) { 
        fflush( NULL );
        batch();
    }      
}



#endif
