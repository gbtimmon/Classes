#ifndef _MYTHREAD_H
#define _MYTHREAD_H

#include <ucontext.h>

#define STACK_SIZE (1024*8)

#define THREAD_IS_STARTED  1
#define THREAD_IS_COMPLETE 2
#define THREAD_HAS_CHILD   4
#define THREAD_YIELD_IND   8
#define THREAD_JOIN       16
#define THREAD_JOIN_ALL   32

typedef ucontext_t ctx_t;

typedef struct Thread {
    int id;

    int child_count;
    int dead_count;

    int flags;
    ctx_t ctx;
    ctx_t return_ctx;

    void(*func)();
    void* args;

    struct Thread* parent;
    struct Thread* after;
    struct Thread* before;
} Thread;

typedef Thread* Queue;

void    _MyThreadPause( Queue * q );
Thread*  _MyThreadPop( Queue* q );
void    _MyThreadPrint( Thread* t );
void    _MyThreadPush( Queue* q, Thread* e ) ;
void    _MyThreadRun() ;
void    _MyThreadSchedule();
Thread* _MyThreadGetCurrent();
void    _MyThreadUnblock(Thread* t);
void    _MyThreadFree(Thread* t);
void    _MyThreadReap();
void    _MyThreadListRemove(Queue* from, Thread* t);
int     _MyThreadQueueLength(Queue * q);
typedef struct Sem {
    int     count;
    Queue q_blocked;
} Sem;

#endif
