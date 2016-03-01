#ifndef JOB_C
#define JOB_C


#define _GNU_SOURCE

#include "job.h"
#include "parse.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define OK     0
#define NOT_OK 1

#define NO     0
#define YES    1

#define DEFAULT_MODE  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define PIPE_READ(x)  x[0]
#define PIPE_WRITE(x) x[1]

#define IS_PARENT(x)           ( x > 0 )
#define IS_CHILD(x)            ( x == 0 )

extern int STDIN_FD;
extern int STDOUT_FD;
extern int STDERR_FD; 

void dispError(){
    if( errno == ENOENT ) { 
        dprintf(STDERR_FD, "command not found\n"); 
    } else if ( errno == EPERM ) {
        dprintf(STDERR_FD, "permission denied\n");
    } else { 
        dprintf(STDERR_FD, "An error has occured, errno %d [%s]\n", errno, strerror(errno));
    }
}
 
int Job_add(Job j, Proc p){
    if( !j )
       return NOT_OK;

    if( !j->head ) {
        j->head = p;
        return OK;
    }

    Proc q = j->head;

    while( q->next != NULL ) {
        q = q->next;
    }
    q->next = p;
    return OK;
}

Proc Proc_new(){

    Proc p = malloc(sizeof(struct prc_t));

    p->cmd         = NULL;
    p->next        = NULL;
    p->in_fd       = -1; 
    p->out_fd      = -1; 
    p->out_inc_err = NO;
    p->builtin     = NULL;
    p->id          = -1;
    p->created     = NO;
    p->exited      = NO;

    return p;
}

Job Job_new(){
 
     Job j = malloc(sizeof(struct job_t)); 
     
     j->head    = NULL;
     j->id      = -1; 
     j->prc_cnt = 0; 
     j->next    = NULL;
     j->last    = NULL;

     return j ; 

}

Job Job_build(Pipe pp ){

    if( !pp ) return (Job) NULL ;
    if( !pp->head) return (Job) NULL;

    Job j = Job_new();
    j->head = NULL;

    Cmd c = pp->head;
    int* pipeline = NULL;
    while( c ) {

        //Allocate the process;//
        Proc p = Proc_new();

        //Set the input;//
        if( pipeline != NULL) {
            p->in_fd = PIPE_READ(pipeline);
            free(pipeline);
            pipeline = NULL;
        } else if ( c->in == Tin ) {

            p->in_fd = open( c->infile, O_RDONLY );
        
            if( p->in_fd < 0 ) {
                dispError();
                free(p);
                Job_free(j);
                return NULL;
            }

        } else {
            p->in_fd = -1;
        }

        //Set the output;//
        if( c->out == Tpipe || c->out == TpipeErr ) {
            pipeline = malloc(sizeof(int) * 2);
            pipe(pipeline);
            p->out_fd = PIPE_WRITE(pipeline);

        } else if( c->out == Tout || c->out == ToutErr ) {
            
            p->out_fd = open( c->outfile, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_MODE);

            if( p->out_fd < 0 ) { 
                free(p);
                Job_free(j);
                return NULL;
            }

        } else if( c->out == Tapp || c->out == TappErr ) { 
            
            p->out_fd = open( c->outfile, O_WRONLY | O_APPEND | O_CREAT, DEFAULT_MODE );

            if( p->out_fd < 0 ) {
                free(p);
                Job_free(j);
                return NULL;
            }

        } else {
           p->out_fd = -1;
        }
        
        //If error is included;//
        if( c->out == TpipeErr || c->out == TappErr || c->out == ToutErr ) { 
            p->out_inc_err = YES;
        }
        //Set the builtin;//
        p->builtin = getBuiltin(c); 
        
        //Wrap up;//
        p->cmd = c;
        Job_add(j, p);
        c = c->next;
    }
    return j;
}

void runLocal( Proc p ) { 

    if( p->in_fd >= 0 ) { 
        close(STDIN_FILENO);
        dup2( p->in_fd, STDIN_FILENO); 
    }
    
  
    if( p->out_fd >= 0 ) {
        close(STDOUT_FILENO);
        dup2( p->out_fd, STDOUT_FILENO); 
        
        if( p->out_inc_err == YES ) {
            close(STDERR_FILENO);
            dup2( p->out_fd, STDERR_FILENO );
        }
    } 

    p->builtin( p->cmd ); 

    close(STDOUT_FILENO);
    dup2(dup(STDOUT_FD), STDOUT_FILENO);

    close(STDERR_FILENO);
    dup2(dup(STDERR_FD), STDERR_FILENO);

    close(STDIN_FILENO);
    dup2(dup(STDIN_FD), STDIN_FILENO);

}

void Job_run(Job j) {
    if(!j) return;   
    if(!j->head) return;

    Proc p = j->head;
  
    if( p->builtin != NULL && p->next == NULL ){
        runLocal( p );
        return;
    }
    
    while( p ) { 

        pid_t pid = fork();
        if(IS_PARENT( pid ) ) { 
 
            if(j->id == -1 ){
                j->id = pid; 
            }

            j->prc_cnt++;
            p->id = pid; 
            p->exited = NO;

            if( p->in_fd >= 0 )close(p->in_fd); 
            if( p->out_fd >= 0 )close(p->out_fd); 


        }  else if ( IS_CHILD(pid) ) {
            if( p->in_fd >= 0 ) { 
                close(STDIN_FILENO);
                dup2( p->in_fd, STDIN_FILENO); 
            }
            
          
            if( p->out_fd >= 0 ) {
                close(STDOUT_FILENO);
                dup2( p->out_fd, STDOUT_FILENO); 
                
                if( p->out_inc_err == YES ) {
                    close(STDERR_FILENO);
                    dup2( p->out_fd, STDERR_FILENO );
                }
            } 
            if( p->builtin ) {
                p->builtin( p->cmd );
                exit(0);
            } else {
                execvp(p->cmd->args[0], p->cmd->args);
                dispError();
                exit(1);
            }
        }
        p = p->next;
    }
}

int Job_close_proc(Job j, pid_t id){
    
   if(!j)       return NOT_OK; 
   if(!j->head) return NOT_OK;
   if(id <= 0)  return NOT_OK; 

   Proc p = j->head;  
 
   while( p ) { 
       if(p->id == id ){
           if(p->in_fd >= 0) close(p->in_fd); 
           if(p->out_fd>= 0) close(p->out_fd); 

           p->exited = YES;
           return OK;
       }
       p = p->next; 
   }
   return NOT_OK; 
}

void Job_join(Job j){
    int stat;
    if( !j ) return; 
    if( !j->prc_cnt > 0 )  return;

    while( j->prc_cnt ) { 
        pid_t pid = waitpid(0, &stat, 0);
        Job_close_proc(j, pid);
        j->prc_cnt--;
    }
}

void Job_free(Job j){

    Proc p = j->head;
    Proc next_p;
    while ( p ) {
        next_p = p->next;
        free(p);
        p = next_p;
    }
    free(j);
}

void Job_kill(Job j){

}

#endif
