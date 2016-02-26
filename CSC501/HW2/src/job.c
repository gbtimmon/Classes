#ifndef JOB_C
#define JOB_C


#include "job.h"
#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define OK     0
#define NOT_OK 1

#define NO     0
#define YES    1

#define PIPE_READ(x)  x[0]
#define PIPE_WRITE(x) x[1]

#define IS_PARENT(x)           ( x > 0 )
#define IS_CHILD(x)            ( x == 0 )

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

    p->cmd     = NULL;
    p->next    = NULL;
    p->in_fd   = -1; 
    p->out_fd  = -1; 
    p->builtin = NULL;
    p->id      = -1;
    p->exited  = YES;

    return p;
}

void Proc_print(Proc p, const char* event){
        printf("     PRC evt[%s] id[%d] arg[%s] in[%d] out[%d] bi[%s]\n",
                event,
                p->id,
                p->cmd->args[0], 
                p->in_fd, 
                p->out_fd, 
               (p->builtin)?"Yes":"No");
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

Job Job_build(Pipe p ){

    if( !p ) return (Job) NULL ;
    if( !p->head) return (Job) NULL;

    Job j = Job_new();
    j->head = NULL;

    Cmd c = p->head;
    int* pipeline = NULL;
    while( c ) {

        //Allocate the process;//
        Proc p = Proc_new();

        //Set the input;//
        if( pipeline != NULL) {
            p->in_fd = PIPE_READ(pipeline);
            free(pipeline);
            pipeline = NULL;
        } else {
            p->in_fd = -1;
        }

        //Set the output;//
        if( c->out == Tpipe || c->out == TpipeErr) {
           pipeline = malloc(sizeof(int) * 2);
           pipe(pipeline);
           p->out_fd = PIPE_WRITE(pipeline);
        } else {
           p->out_fd = -1;
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

void Job_run(Job j) {

    if(!j) return;   
    if(!j->head) return;

    Proc p = j->head;
  
    
    while( p ) { 
        
        pid_t pid = fork();

        if(IS_PARENT( pid ) ) { 
 
            if(j->id == -1 ){
                j->id = pid; 
            }
            j->prc_cnt++;
            p->id = pid; 
            p->exited = NO;

            close(p->in_fd); 
            close(p->out_fd); 

        }  else if ( IS_CHILD(pid) ) {
            if( p->in_fd >= 0 ) { 
                dup2( p->in_fd, STDIN_FILENO); 
            }
            
          
            if( p->out_fd >= 0 ) {

                int fd = ( p->cmd->out == ToutErr || p->cmd->out == TappErr || p->cmd->out == TpipeErr ) 
                    ? STDERR_FILENO
                    : STDOUT_FILENO;

                dup2( p->out_fd, fd); 
            }

            execvp(p->cmd->args[0], p->cmd->args);
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
           close(p->in_fd); 
           close(p->out_fd); 

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
        printf("Pid %d return\n", (int) pid);
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

void
Job_print(Job j, const char* event){
    printf("Job\n");
    if( !j ) {
        printf("    <NULL>\n");
        return;
    }
    Proc p = j->head;

    while( p ) {
        Proc_print(p, event);
        p = p->next;
    }
}
#endif
