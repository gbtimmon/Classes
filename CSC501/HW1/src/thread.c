#ifndef THREAD_C
#define THREAD_C
 
#include <stdio.h>
#include <string.h>
#include <logmacros.h>
#include <stdlib.h>
#include <ucontext.h>
#include <thread.h>

/*
 * Create a new thread object
 */
static curr_id = 0; 
Thread* Thread_new(Thread* parent, void(*f)(void*), void* args) { 
    tcs("> Thread*  Thread_new(char* nm , int id) ");

    Thread* t      = (Thread*) malloc(sizeof(Thread));
    t->func        = (void(*)()) f; 
    t->args        = args; 
    t->id          = curr_id++; 
    t->next        = NULL; 
    t->last        = NULL;
    t->flags       = 0;
    t->child_count = 0; 

    parent->child_count++; 

    return t; 
};

/*
 * Pop a thread from the thread list started at 
 * the Thread object pointed at by 'queue'
 *
 * If the queue has one element it will leave
 * the queue empty
 */
Thread* Thread_pop(Thread** queue){ 
    tcs("> Thread*  pop_Thread(Thread** queue)");
     
    Thread* c = *queue; 
    if( c  == NULL ) return NULL;

    while( c->last != NULL ) c = c->last; 
   
    if(c->next == NULL ) *queue = NULL;
    else   c->next->last = NULL;

    return c; 
};

/*
 * Swap the current context for the context of the thread 
 * passed by this function. 
 *
 * If the thread is new ( !THREAD_IS_STARTED ) the the context
 * must be built.
 *
 * If the thread has already started then we simple resume the 
 * context
 *
 * Threads are set to return to the calling process when completed
 * or yeiled. 
 */
Thread* Thread_run(Thread* t) { 
    tcs("> Thread*  Thread_run(Thread* t)");

    if( t->flags & THREAD_IS_STARTED ) {
        swapcontext(t->return_ctx, t->ctx);
    } else {
        t->flags |= THREAD_IS_STARTED;
        t->return_ctx            = (ctx_t*) malloc(sizeof(ctx_t));
        t->ctx                   = (ctx_t*) malloc(sizeof(ctx_t));
        getcontext(t->ctx); 
        t->ctx->uc_stack.ss_sp   = malloc(STACK_SIZE);
        t->ctx->uc_stack.ss_size = STACK_SIZE;
        t->ctx->uc_link          = t->return_ctx; 
        makecontext(t->ctx, t->func, 1,  t->args); 
        swapcontext(t->return_ctx, t->ctx);
    }
};

void Thread_push(Thread** queue, Thread* ele) { 
    tcs("> void     Thread_push(Thread** queue, Thread* ele)");
    if( *queue == NULL ) {
        *queue = ele;     
    } else { 
        (*queue)->next = ele; 
        ele->last    = *queue; 
        ele->next    = NULL;
        *queue       = ele;     
    }
};
void Thread_join_all(Thread** q, Thread* me) { 
    tcs("> void     Thread_join_all(Thread** q, Thread* me)");    
}

void Thread_join(Thread** q, Thread* me,  Thread* you ) {
    tcs("> void     Thread_join(Thread** q,  Thread * w )");
    while(!(you->flags & THREAD_IS_COMPLETE)){
        Thread_pause(q,me);
    }
};

int Thread_queue_length( Thread* head ) {
    tcs("> int Thread_queue_length( Thread* head )");
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
    tcs("> void   schedule_Thread( void )");

    while(*queue != NULL){
        info("schedule loop");
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
        if(t->flags & THREAD_YIELD_IND) {
            t->flags &= ~THREAD_YIELD_IND;   
        } else {
            t->flags |= THREAD_IS_COMPLETE; 
        }
        *running = NULL;
    }
     

};

void Thread_pause(Thread** q, Thread* t){
    tcs("> void     Thread_pause(Thread** t)");
    Thread_push(q, t);
    t->flags |= THREAD_YIELD_IND;
    swapcontext(t->ctx, t->return_ctx);
}

void Thread_print( Thread* t ){
    printf("Id     : %d\n", t->id);
    printf("next Id: %d\n", (t->next)? t->next->id : -1);
    printf("last Id: %d\n", (t->last)? t->last->id : -1);
    printf("ctx    : %s\n", (t->ctx)? "Yes" :"No");
    printf("ret_ctx: %s\n", (t->return_ctx)? "Yes" :"No");

};
#endif
