/*
 * myThread.c++
 *
 *  Created on: Jan 20, 2016
 *      Author: u236862
 */
#ifndef MYTHREAD_C
#define MYTHREAD_C
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>
#include <mythread.h>

#define DEBUG
#ifdef DEBUG
    #define debug(x) printf("[DEBUG] "); printf(x); printf("\n")
#else
    #define debug(x)
#endif

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif


static Thread* ready_queue;
static Thread* running_thread;
static int thread_count = 0;

void f1 (void* args);
int a = 4;
int b = 7; 
void f0 (void* args) {
    printf("    This is thread 1 : %d\n", *((int*) args));
    MyThreadCreate(f1, &a);
    MyThreadCreate(f1, &b);
};
void f1 (void* args) {  
     int* g = (int*) args;
     int i; 
     for( i = 0; i < (*g)*2; i++) { 
         printf("Thread %d : iteration %d\n", running_thread->id, i);
         if( i == (*g) ) MyThreadYeild();
     }

};
typedef ucontext_t ctx_t; 

typedef struct Thread {
    char* name; 
    int id; 

    int  isRunning; 
    ctx_t* ctx;     
    ctx_t* return_ctx; 

    void(*func)();      
    void* args;

    struct Thread* next; 
    struct Thread* last;
} Thread; 

Thread* new_Thread(char* nm, int id, void(*f)(void*), void* args) { 
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

void print_Thread( Thread* t ) { 
    debug("> void print_Thread( Thread* t )");

    printf("Thread\n");
    printf("   id        : %d\n", t->id);
    printf("   name      : %s\n", t->name);
    printf("   next      : %s\n", (t->next) ? t->next->name : "NULL"); 
    printf("   last      : %s\n", (t->last) ? t->last->name : "NULL");     
};

Thread* pop_Thread(Thread** queue){ 
    debug("> Thread*  pop_Thread(Thread** queue)");
     
    Thread* c = *queue; 

    if( c  == NULL ) return NULL;

    while( c->last != NULL ) c = c->last; 
   
    if(c->next == NULL ) *queue = NULL;

    else   c->next->last = NULL;

    return c; 
}

Thread* pause_Thread(Thread* t){
     swapcontext(t->ctx, t->return_ctx);
};

Thread* run_Thread(Thread* t) { 
    debug("> Thread*  run_Thread(Thread* t)");

    if(t->isRunning == 0 ) {
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

void push_Thread(Thread** queue, Thread* ele) { 
    debug("> void     push_Thread(Thread** queue, Thread* ele)");
    if( *queue == NULL ) {
        *queue = ele;     
    } else { 
        (*queue)->next = ele; 
        ele->last    = *queue; 
        *queue       = ele;     
    }
};

void schedule_Thread( void ){ 
    debug("> void   schedule_Thread( void )");

    while(ready_queue != NULL){
        //Get the next out of the queue; 
        Thread* t = pop_Thread(&ready_queue);
        
        //pop it from the queue; 
        if(t->next == NULL){
            ready_queue = NULL;
        } else {
           t->next->last = NULL;
        }
 
        running_thread = t; 
        run_Thread(t);        
        running_thread = NULL;
    }
     
};

void wrap_Thread( void(*f)(void*), void* a ){ 
    debug("> void mt_wrapper( void(*f)(void*), void* a )");
    MyThreadExit(); 
};

void MyThreadInit( void(*f)(void*), void* args ) {  
    debug("> void     MyThreadInit(void(*f)(void*), void* args)");
    push_Thread( &ready_queue, new_Thread("First", thread_count++, f, args) );
    schedule_Thread(); 
};

MyThread MyThreadCreate( void(*f)(void*), void* args ) { 
    debug("> MyThread MyThreadCreate(void(*start_funct)(void*), void* args)"); 
    push_Thread( &ready_queue,  new_Thread("MTC", thread_count++, f, args) ); 
}; 

void MyThreadYeild( void ) { 
    debug("> void    MyThreadYeild( void )");

    Thread* me = running_thread; 
    running_thread = NULL;
    swapcontext(me->ctx, me->return_ctx);

};

void MyThreadExit(void) { 
    debug("> void    MyThreadExit(void)");
    (void*) 0; 
};



int k = 9; 
int main ( int argc, void* args){
    debug("> int      main ( int argc, void* args)");
    MyThreadInit(&f0, &k);
};

#endif
