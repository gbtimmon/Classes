#ifndef THREAD_C
#define THREAD_C
 
#include <stdio.h>
#include <string.h>
#include <logmacros.h>
#include <stdlib.h>
#include <ucontext.h>
#include <thread.h>



Thread* Thread_new(char* nm, int id, void(*f)(void*), void* args) { 
    debug("> Thread*  new_Thread(char* nm , int id) ");

    int l = strlen(nm);

    Thread* t    = (Thread*) malloc(sizeof(Thread));
    t->name      = (char*) malloc(sizeof(char*) * l );
    t->func      = (void(*)()) f; 
    t->args      = args; 
    t->id        = id; 
    t->next      = NULL; 
    t->last      = NULL;
    t->isRunning = 0; 

    strcpy(t->name, nm);
    return t; 
};

void Thread_print( Thread* t ) { 
    debug("> void print_Thread( Thread* t )");

    printf("Thread\n");
    printf("   id        : %d\n", t->id);
    printf("   name      : %s\n", t->name);
    printf("   next      : %s\n", (t->next) ? t->next->name : "NULL"); 
    printf("   last      : %s\n", (t->last) ? t->last->name : "NULL");     
};

Thread* Thread_pop(Thread** queue){ 
    debug("> Thread*  pop_Thread(Thread** queue)");
     
    Thread* c = *queue; 
    if( c  == NULL ) return NULL;

    while( c->last != NULL ) c = c->last; 
   
    if(c->next == NULL ) *queue = NULL;
    else   c->next->last = NULL;

    return c; 
};

Thread* Thread_pause(Thread* t){
     swapcontext(t->ctx, t->return_ctx);
};

Thread* Thread_run(Thread* t) { 
    debug("> Thread*  Thread_run(Thread* t)");

    if(t->isRunning == 0 ) {
        t->isRunning = 1; 
        debug("Starting Thread");
        t->return_ctx            = (ctx_t*) malloc(sizeof(ctx_t));
        t->ctx                   = (ctx_t*) malloc(sizeof(ctx_t));
        getcontext(t->ctx); 
        t->ctx->uc_stack.ss_sp   = malloc(STACK_SIZE);
        t->ctx->uc_stack.ss_size = STACK_SIZE;
        t->ctx->uc_link          = t->return_ctx; 
        makecontext(t->ctx, t->func, 1,  t->args); 
        swapcontext(t->return_ctx, t->ctx);
    } else { 
        debug("Resuming Thread");
        swapcontext(t->return_ctx, t->ctx);
    }
};

void Thread_push(Thread** queue, Thread* ele) { 
    debug("> void     push_Thread(Thread** queue, Thread* ele)");
    if( *queue == NULL ) {
        *queue = ele;     
    } else { 
        (*queue)->next = ele; 
        ele->last    = *queue; 
        *queue       = ele;     
    }
};

int Thread_queue_length( Thread* head ) {
    debug("> int Thread_queue_length( Thread* head )");
    int c = 0; 
    Thread* t = head; 

    if( t == NULL ) return c; 
    c++;
    while(t->last != 0) {
       c++;
       t = t->last;
    }
    return c; 
};


void Thread_scheduler( Thread** queue, Thread** running){ 
    debug("> void   schedule_Thread( void )");

    while(*queue != NULL){
        infof("Length of queue", Thread_queue_length( *queue ) );
        //Get the next out of the queue; 
        Thread* t = Thread_pop(queue);
        
        //pop it from the queue; 
        if(t->next == NULL){
            *queue = NULL;
        } else {
           t->next->last = NULL;
        }
 
        *running = t; 
        Thread_run(t);        
        *running = NULL;
    }
     
};
#endif
