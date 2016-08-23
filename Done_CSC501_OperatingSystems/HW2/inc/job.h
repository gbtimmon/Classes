#ifndef JOB_H
#define JOB_H

#include "parse.h"
#include "builtins.h"
#include <sys/types.h>


struct prc_t 
{
    Cmd cmd; 
    struct prc_t* next; 
    int in_fd; 
    int out_fd; 
    int out_inc_err;
    int exited;
    int created;
    Builtin builtin;
    pid_t id; 
}; 

struct job_t { 
     struct prc_t *head; 
     pid_t        id;
     int          prc_cnt; 
     struct job_t *next; 
     struct job_t *last; 
     int          end;
}; 

typedef struct prc_t *Proc;
typedef struct job_t *Job; 

Job Job_build(Pipe); 
void Job_free(Job);
void Job_run(Job);
void Job_join(Job);
void Job_kill(Job);
#endif
