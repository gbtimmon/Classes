#ifndef THREAD_H
#define THREAD_H
 
#include <ucontext.h>

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif

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

Thread* Thread_new(char* nm, int id, void(*f)(void*), void* args) ; 
void    Thread_print( Thread* t ) ; 
Thread* Thread_pop(Thread** queue); 
Thread* Thread_pause(Thread* t);
Thread* Thread_run(Thread* t) ; 
void    Thread_push(Thread** queue, Thread* ele) ; 
void Thread_schedule( Thread** queue, Thread** running);

#endif
