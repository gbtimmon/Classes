/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: Simple driver program for ush's parser
 *
 *  Author...........: Vincent W. Freeh
 *
 *****************************************************************************/

#define _GNU_SOURCE
#include <builtins.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <parse.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define OK                     0
#define NOT_OK                 -1

#define DEFAULT_MODE 	       S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH 
#define DEFAULT_BUFFER_SIZE    2048
#define DEFAULT_CONFIG_FILE    "/.ushrc"

#define prompt()               printf("armadillo%%"); fflush(stdout);
#define IS_PARENT(x)           ( x > 0 )
#define IS_CHILD(x)            ( x == 0 )

#define PIPE_READ(x)           ( x[0] )
#define PIPE_WRITE(x)          ( x[1] )


/*
 * dup_file 
 *
 * Procedure to redirect an existing OS file to a std file 
 * channel. 
 */

static int runCmd(int, int*, Cmd);

int dup_file(const char* fname, int flags, int dest_fd, int* orig_fd_pt ) {

    int orig_fd, new_fd; 

    if( orig_fd_pt != NULL) { 
        if( (orig_fd = dup(dest_fd) ) < 0 ) { 
            fprintf(stderr, "ush: Failed to dup file %d, errno %d\n [%s]\n", dest_fd, errno, strerror(errno));
            return NOT_OK; 
        }
    }
    
    if( ( new_fd = open(fname, flags, DEFAULT_MODE) ) < 0 ){
        fprintf(stderr, "ush: Failed to open file %s, errno %d\n [%s]\n", fname, errno, strerror(errno));
        return NOT_OK; 
    }

    if ( dup2(new_fd, dest_fd) < 0 ) { 
        fprintf(stderr, "ush: Failed to redirect %s to %d\n", fname, dest_fd);
        return NOT_OK; 
    }
    
    if ( orig_fd_pt != NULL) *orig_fd_pt = orig_fd; 
    return OK; 
}

/*
 * Dud wrapper without a file open.
 * Used for pipes. 
 */
int dup_pipe( int src_fd, int dest_fd, int* orig_fd_pt) { 
    int orig_fd; 
    
    perror("A"); 
    if( orig_fd_pt != NULL ) {
    perror("B"); 
        if( (orig_fd = dup(dest_fd) ) < 0 ) {
    perror("C"); 
            fprintf(stderr, "ush: Failed to dup file %d, errno %d\n [%s]\n", dest_fd, errno, strerror(errno));
    perror("D"); 
            return NOT_OK;
        }
    }

    if ( dup2(src_fd, dest_fd) < 0 ) {
    perror("E"); 
        fprintf(stderr, "ush: Failed to redirect %d to %d, errno %d\n [%s]\n", src_fd, dest_fd, errno, strerror(errno));
    perror("F"); 
        return NOT_OK;
    perror("G"); 
    }

    if( orig_fd_pt != NULL ) *orig_fd_pt = orig_fd;
    perror("H"); 

    return OK;

}

int directStdIn(int * p, Cmd c, int* orig_fd_pt){
    if( c->in == Tin) {
        if( dup_file( c->infile, O_RDONLY, STDIN_FILENO, orig_fd_pt) != OK ) { 
            return NOT_OK;   
        } 
        return OK; 
    } else if (p != NULL ) { 
        if ( dup_pipe( PIPE_READ(p), STDIN_FILENO, orig_fd_pt ) != OK ) { 
            return NOT_OK; 
        }
        return OK; 
    } else {
        if(orig_fd_pt) *orig_fd_pt = -1;
        return OK; 
    }
}

int directOut(int* p, Cmd c){
    if( c->out == Tnil)
        return OK; 
    switch( c->out ) {

        case Tpipe :
            fprintf(stderr, "aAAAA\n"); fflush(NULL);
            return dup_pipe( PIPE_WRITE(p), STDOUT_FILENO, NULL );
            break;
        default :
            fprintf(stderr, "Unimplemented token\n"); 
    }


    return OK;
}
    /*
    if( c->out != Tnil ) { 
        switch( c->out ) {
            case Tout :    // >
                dup_file(c->outfile, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO );
                break;
            case Tapp :   // >>
                dup_file(c->outfile, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO ) ;
                break;
            case ToutErr: // >&
                dup_file(c->outfile, O_WRONLY | O_CREAT , STDERR_FILENO );
                break;
            case TappErr: // >>&
                dup_file(c->outfile, O_WRONLY | O_CREAT | O_APPEND, STDERR_FILENO ) ;
                break;
            default : 
                fprintf(stderr, "Shouldnt be here 4434\n");
                exit(NOT_OK);
        }
    } 
}**/

static int runLocal(int * p, Cmd c, Builtin b){
  
    int fd_to_return; //This will be the original std_in fd. 
                      // will need to return to this to not break
                      // the shell after a builtin. 
    if ( directStdIn(p, c, &fd_to_return) != OK ) { 
        return NOT_OK; 
    }

    if( b(c) != OK ) {
        return NOT_OK; 
    }

    if( fd_to_return >= 0 ) {
        if( dup2( fd_to_return, STDIN_FILENO ) < 0 ) {
            fprintf(stderr, "Failed to reclaim stdin, Fatal Error\n"); 
            exit(NOT_OK); 
        }
    }

    return OK;
}

static int buildPipe(int* p, Cmd c){
    if( c->out != Tpipe && c->out != TpipeErr ) 
        return OK; 

    if( pipe(p) < 0 ) {
        fprintf(stderr, "ush: failed to create a pipe, errno %d\n[%s]\n", errno, strerror(errno));
        return NOT_OK; 
    }
    return OK; 
}

static int runFork(int chain, int * p, Cmd c, Builtin b){
    
    int new_pipe[2]; 
    buildPipe(new_pipe, c); 

    pid_t pid = fork();

    if ( IS_PARENT(pid) ) { 
         if(c->next != NULL){
             if( runCmd(chain+1, new_pipe, c->next) != OK ){
                 fprintf(stderr, "ush: command failed \n"); 
                 kill(pid, SIGTERM); 
                 return NOT_OK; 
             }
         }
   
         int status; 
         wait(&status); 
         if( WEXITSTATUS(status) != 0 ) { 
             fprintf(stderr, "ush: command failed\n");
             return NOT_OK; 
         } else {
             return OK; 
         }
    } 
    if ( IS_CHILD(pid) ) { 
        directStdIn(p, c, NULL);
        directOut(p, c);
        execvp(c->args[0], c->args); 
        return OK; 
    }
    return OK;
}

static int runCmd(int chain, int * p , Cmd c) { 

    if( ! c ) 
        return NOT_OK; 
    if( !strcmp( c->args[0], "end") )    
        return NOT_OK; 

    int isLast = (c->next == NULL); 
    Builtin b = (getBuiltin(c->args[0])); 
    
    if(p != NULL && c->in == Tin) { //if not first but has inredirect.
        fprintf(stderr, " %s: Improper input redirection", c->args[0]);
        return NOT_OK;
    }

    if(isLast && b != NULL ) { // we arent gonna fork 
         return runLocal(p, c, b); 
    } else {                   // lets run in a child process. 
         return runFork(chain, p, c, b); 
    }
}

const char* getHome() { 
    return (getenv("HOME") == NULL)
        ? getpwuid( getuid() )->pw_dir
        : getenv("HOME"); 
}

/*
 * Start the shell
 * init steps are 
 *  1.) try to open the .ushrc at $HOME
 *  2.) execute the config file
 */

int init() { 
    char file_path [DEFAULT_BUFFER_SIZE];
    strcpy(file_path, getHome());
    strcat(file_path, DEFAULT_CONFIG_FILE);
    
    int old_stdin;
    dup_file( file_path, O_RDONLY, STDIN_FILENO, &old_stdin ); 
    while( 1 ){
        Pipe p = parse();
        while( p != NULL ){
            int rc = runCmd(0, NULL, p->head);
            if( rc != OK ) {
                
                dup2( old_stdin, STDIN_FILENO);
                return OK;
            } 
            p = p->next; 
         }
    }
}

/*
 *mainLoop, where the magic happens 
 */
int mainLoop() {
    while(1){
        prompt();
        Pipe p = parse();
        while ( p != NULL ) {
            runCmd(0, NULL, p->head);
            p = p->next;
        }
        freePipe(p);
    }
}

int main(int argc, char *argv[])
{
    printf("DEBUG> %s\n", getHome());
    printf("EFAIL> %d\n", NOT_OK); 
    int rc; 
    rc = init();
    //if (rc == ERR) 
    rc = mainLoop();
    //if (rc == ERR)
}
