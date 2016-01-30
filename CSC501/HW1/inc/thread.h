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

    struct Thread* next; 
    struct Thread* last;
} Thread; 

void    Thread_join(Thread** q, Thread* me, Thread* you);
void    Thread_join_all(Thread** q, Thread* me);
Thread* Thread_new(Thread* parent, void(*f)(void*), void* args); 
void    Thread_pause(Thread** queue, Thread* thread);
Thread* Thread_pop(Thread** queue); 
void    Thread_print(Thread* t);
void    Thread_push(Thread** queue, Thread* ele) ; 
Thread* Thread_run(Thread* t) ; 
void    Thread_schedule( Thread** queue, Thread** running);

#endif
