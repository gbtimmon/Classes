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
static int     curr_id    = 0; 
static Queue q_running  = NULL;
static Queue q_ready    = NULL;
static Queue q_blocked  = NULL;
static Queue q_reapable = NULL;
  
Thread* Thread_new(void(*f)(void*), void* args) { 
    tcs("Thread_new(char* nm , int id) ");

    Thread* t      = (Thread*) malloc(sizeof(Thread));

    t->func        = (void(*)()) f; 
    t->args        = args; 
    t->id          = curr_id++; 
    t->next        = NULL; 
    t->last        = NULL;
    t->flags       = 0;
    t->child_count = 0; 
    t->parent      = q_running;

    if(t->parent != NULL ) 
        t->parent->child_count++; 

    Thread_push(&q_ready, t);
    return t; 
};

/*
 * Pop a thread from the thread list started at 
 * the Thread object pointed at by 'queue'
 *
 * If the queue has one element it will leave
 * the queue empty
 */
Thread* Thread_pop(Queue* q){ 
    tcs("Thread_pop(Thread* c)");
    Thread* c = *q;

    if( c  == NULL ) 
        return NULL;
    while( c->last != NULL ) {
        c = c->last; 
     }
    if(c->next == NULL ) {
       *q = NULL;
    }else   {
        c->next->last = NULL;
    }
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
Thread* Thread_run() { 
    tcs("Thread_run()");
    Thread* t = q_running; 
    if( t->flags & THREAD_IS_STARTED ) {
        swapcontext(t->return_ctx, t->ctx);
    } else {
        t->flags                |= THREAD_IS_STARTED;
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

/*
 * Add a thread to a list/queue of lists. 
 */
void Thread_push(Queue* q, Thread* e) { 
    tcs("Thread_push(Thread** queue, Thread* ele)");
    if( *q == NULL ) {
        *q = e;     
    } else { 
        *q)->next = e; 
        e->last    = *q; 
        e->next    = NULL;
        *q         = e;     
    }
};

void Thread_join_all() { 
    tcs("Thread_join_all(Thread** q, Thread* me)");    
    while(q_running->child_count > 0 ){
        Thread_pause_and_queue(&q_ready);   
    }
}

void Thread_join(Thread* you) {
    tcs("Thread_join(Thread* you )");
    while(!(you->flags & THREAD_IS_COMPLETE)){
        Thread_pause_and_queue(&q_ready);
    }
};

int Thread_queue_length( Queue* q ) {
    tcs("Thread_queue_length( Thread* head )");
    int c = 0; 
    Thread* t = *q; 

    if( t == NULL ) return c; 
    c++;
    while(t->last != 0) {
       c++;
       t = t->last;
    }
    return c; 
};

void Thread_scheduler(){ 
    tcs("Thread_scheduler");

    while((q_ready != NULL) || (q_blocked != NULL)){
        info("schedule loop");
        //Get the next out of the queue; 
        q_running = Thread_pop(&q_ready); 
        if(q_running != NULL){
            Thread_run();        
      
          //Thread came to schedule incomplete. 
            if( q_running->flags &   THREAD_YIELD_IND ) {
                q_running->flags &= ~THREAD_YIELD_IND;   
          //Thread came in completed : run thread reaping. 
            } else {
                q_running->flags |= THREAD_IS_COMPLETE; 
                if( q_running->parent != NULL )
                    q_running->parent->child_count--;
                Thread_push(&q_reapable, q_running); 
            }
        }
    }
};

void Thread_yeild(){
    tcs("Thread_yeild()");
    Thread_pause_and_queue(&q_ready);
};

/*
 *Queue the current thread to the queue specified
 */
void Thread_pause_and_queue(Queue* q){
    tcs("Thread_pause_and_queue(Queue * q)");
    Thread_push(q, q_running);
    q_running->flags |= THREAD_YIELD_IND;
    swapcontext(q_running->ctx, q_running->return_ctx);
};

void Thread_print( Thread* t ){
    if( t == NULL ) t = q_running; 
    tcs("Thread_print( Thread* t )");
    info("Id     : %d", t->id);
    info("next Id: %d", (t->next)? t->next->id : -1);
    info("last Id: %d", (t->last)? t->last->id : -1);
    info("ctx    : %s", (t->ctx)? "Yes" :"No");
    info("ret_ctx: %s", (t->return_ctx)? "Yes" :"No");
};

Thread* Thread_getCurrent(){
    return q_running; 
};

Sem* Sem_new(int i){
    Sem* s = malloc(sizeof(Sem));
    s->count = i;
    s->q_blocked;
    return s; 
};

void Sem_sig(Sem* s){
    if(s->count < 0 ) { 
       Thread_push(&q_ready, Thread_pop(&s->q_blocked));
    }
    s->count++;
};

void Sem_wait(Sem* s){
    s->count--;
    if(s->count <0){
        Thread_pause_and_queue(&s->q_blocked);
    }
};

int Sem_destroy(Sem* s){
    if( s->q_blocked != NULL)
       return -1; 
    free(s); 
    return 0;          
}
#endif
