#ifndef THREAD_H
#define THREAD_H
 
#include <ucontext.h>

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif

#define THREAD_IS_STARTED  1
#define THREAD_IS_COMPLETE 2
#define THREAD_HAS_CHILD   4
#define THREAD_YIELD_IND   8


typedef ucontext_t ctx_t; 

typedef struct Thread {
    int id; 
 
    int child_count; 

    int flags; 
    ctx_t* ctx;     
    ctx_t* return_ctx; 

    void(*func)();      
    void* args;

    struct Thread* parent;
    struct Thread* next; 
    struct Thread* last;
} Thread; 

Thread* Thread_getCurrent();
void    Thread_join(Thread* you);
void    Thread_join_all();
Thread* Thread_new(void(*f)(void*), void* args); 
void    Thread_pause();
Thread* Thread_pop(Thread** q); 
void    Thread_print(Thread* t);
void    Thread_push(Thread** q, Thread* e) ; 
Thread* Thread_run() ; 
void    Thread_schedule();

#endif
