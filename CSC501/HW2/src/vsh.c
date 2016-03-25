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

#define RC_FILE      "/.ushrc"
#define DEFAULT_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define IS_PARENT(x) ( x >  0 )
#define IS_CHILD(x)  ( x == 0 )

#define NEW_PIPE()   malloc(sizeof(int) * 2)
#define PIPE_RD(x)   x[0]
#define PIPE_WT(x)   x[1]

#define IS_END(x)    !strcmp(x->head->args[0], "end")
#define CHUNK_SIZE   32 
#define BUFFER_SIZE  256
#define LOG(x)       dprintf( STDERR_FD, "[LOG] :  %s\n", x);

int STDIN_FD; 
int STDOUT_FD; 
int STDERR_FD; 

static char hostname[BUFFER_SIZE] = "" ; 
static char rc_path[BUFFER_SIZE]  = "";


void sig_handler( int signo ) {
    dprintf( STDOUT_FD, "\n%s%%", hostname);
}

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

int doPipe(Pipe p){
    Pipe q = p;
    while(q){
        Job j = Job_build(q);
        Job_run(j);
        Job_join(j);
        Job_free(j);
        q = q->next;
    }
    return OK;
}

int batch() {
    while(1){
        Pipe p = parse();
        if ( p ) {
           if( !strcmp( "end",  p->head->args[0] ) )
               return OK;
           doPipe( p );
        }
    }
    return NOT_OK;
}

int main(int argc, char** args, char** env){

    STDIN_FD  = dup( STDIN_FILENO  );
    STDOUT_FD = dup( STDOUT_FILENO ); 
    STDERR_FD = dup( STDERR_FILENO ); 
    
    signal( SIGINT, sig_handler);
    signal( SIGQUIT, sig_handler);    

    gethostname(hostname, BUFFER_SIZE);

    int rc_fd = open( getRCPath(), O_RDONLY); 
    if( rc_fd < 0 ) {
        dprintf( STDERR_FD, "Failed to open config %s. Errno %d [%s]\n",  getRCPath(), errno, strerror(errno));
    }else {
        int save_real_stdin = STDIN_FD;
        STDIN_FD = rc_fd;

        close(STDIN_FILENO);
        dup2( rc_fd, STDIN_FILENO); 

        batch();

        close(STDIN_FILENO);
        STDIN_FD = save_real_stdin; 
        dup2( dup(STDIN_FD), STDIN_FILENO);
    }
    
    int isa = isatty(STDIN_FILENO);
    while(1){
        if( isa ) dprintf(STDOUT_FD, "%s%%", hostname);
        fflush( NULL );
        Pipe p = parse();
        if ( p ) {
           if( !strcmp( "end",  p->head->args[0] ) )
               return OK;
           doPipe( p );
        }
    }

}
#endif
