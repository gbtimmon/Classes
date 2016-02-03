#ifndef THREAD_C
#define THREAD_C
 
#include <stdio.h>
#include <string.h>
#include <logmacros.h>
#include <stdlib.h>
#include <ucontext.h>
#include <mythread.h>
#include <_mythread.h>

#define NOOP() do {} while(0)
/*
 * Create a new thread object
 */
static int     curr_id    = 1;
static Queue q_running  = NULL;
static Queue q_ready    = NULL;
static Queue q_blocked  = NULL;
static Queue q_reapable = NULL;
  
void MyThreadStatus (){
    printf("READY : %d\n, BLOCKED : %d\n, REAP : %d\n", _MyThreadQueueLength(&q_ready), _MyThreadQueueLength(&q_blocked) , _MyThreadQueueLength(&q_reapable));
}
MyThread MyThreadCreate(void(*f)(void*), void* args) { 
    tcs("MyThreadCreate(char* nm , int id) ");

    Thread* t      = (Thread*) malloc(sizeof(Thread));

    t->func        = (void(*)()) f; 
    t->args        = args; 
    t->id          = curr_id++; 
    t->after       = NULL; 
    t->before      = NULL;
    t->flags       = 0;
    t->child_count = 0; 
    t->dead_count  = 0;
    t->parent      = q_running;

    if(t->parent != NULL ) 
        t->parent->child_count++; 

    _MyThreadPush(&q_ready, t);
    return t; 
};

/*
 * Pop a thread from the thread list started at 
 * the Thread object pointed at by 'queue'
 *
 * If the queue has one element it will leave
 * the queue empty
 */
Thread* _MyThreadPop(Queue* q){ 
    tcs("_MyThreadPop(Thread* c)");
    Thread* head = *q; 
    if(head) {
        *q = head->after;
    }
    head->before = NULL;
    head->after  = NULL;
    return head; 
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
void _MyThreadRun() { 
    tcs("_MyThreadRun()");
    Thread* t = q_running; 
    if( t->flags & THREAD_IS_STARTED ) {
        swapcontext(&t->return_ctx, &t->ctx);
    } else {
        t->flags                |= THREAD_IS_STARTED;
        //t->return_ctx            = malloc(sizeof(ctx_t));
        //t->ctx                   = malloc(sizeof(ctx_t));
        getcontext(&t->ctx); 
        t->ctx.uc_stack.ss_sp   = malloc(STACK_SIZE);
        t->ctx.uc_stack.ss_size = STACK_SIZE;
        t->ctx.uc_link          = &t->return_ctx; 
        makecontext(&t->ctx, t->func, 1,  t->args); 
        swapcontext(&t->return_ctx, &t->ctx);
    }
};

/*
 * Add a thread to a list/queue of lists. 
 */
void _MyThreadPush(Queue* q, Thread* e) { 
    tcs("_MyThreadPush(Thread** queue, Thread* ele)");

    Thread* cur = *q;
    if( cur == NULL ) { //If only one of the queue;
        *q = e;     
        e->before  = NULL;
        e->after   = NULL;
       
    } else {           //If going as first in a populated queue; 
        while( cur->after != NULL ) cur = cur->after;
        cur->after = e;
        e->before = cur;
        e->after = NULL;
    }

};

void MyThreadJoinAll() { 
    tcs("MyThreadJoinAll()");    
    Thread * me = q_running; 
 
    if( me->child_count <= me->dead_count ) 
        return; 

    me->flags |= THREAD_JOIN_ALL;
    _MyThreadPause(&q_blocked);
};

int MyThreadJoin(MyThread thread) {
    tcs("MyThreadJoin(MyThread you )");
    Thread* me  = (Thread*) q_running;
    Thread* you = (Thread*) thread;

    if( me != you->parent )
        return -1;
   
    if(you->flags |= THREAD_IS_COMPLETE)
        return 0;

    you->flags |= THREAD_JOIN;
    _MyThreadPause(&q_blocked);
    return 0;
};

int _MyThreadQueueLength( Queue* q ) {
    tcs("_MyThreadQueueLength( Queue* q )");
    int c = 0; 
    Thread* t = *q; 

    if( t == NULL ) return c; 
    c++;
    while(t->after != NULL ) {
       c++;
       t = t->after;
    }
    return c; 
};

void _MyThreadScheduler(){ 
    tcs("_MyThreadScheduler");

    while((q_ready != NULL) || (q_blocked != NULL)){
        info("schedule loop");

        //Get the after out of the queue; 
        Thread* r = _MyThreadPop(&q_ready);
        q_running = r; 

        if(r != NULL){
            _MyThreadRun();        

            //Thread came to schedule incomplete, dont perform any reaping.
            if( r->flags &   THREAD_YIELD_IND ) {
                r->flags &= ~THREAD_YIELD_IND;   
                continue; 
            }   
            //Set Thread Complete
            r->flags |= THREAD_IS_COMPLETE; 

            //Let parent know one of its children has died. How sad. 
            if( r->parent != NULL ){
                r->parent->dead_count++;
                if( r->parent->flags & THREAD_JOIN_ALL ) {
                    if( r->parent->child_count <= r->parent->dead_count ) {
                        _MyThreadUnblock(r->parent);
                    }
                }
            }
                 
            //If Mama was waiting on me to die, let her know. 
            if( r->flags & THREAD_JOIN ){
                _MyThreadUnblock(r->parent);                    
            }
            _MyThreadPush(&q_reapable, r); 
        }else{
           perror("Something wrong has happened!");
        }
        _MyThreadReap();
    }
};

void _MyThreadFree(Thread* t){
    tcs("_MyThreadFree(Thread* t)");
    
    free(t->ctx.uc_stack.ss_sp);
    free(t);
};

void _MyThreadReap(){
     tcs("_MyThreadReap()");
     Thread* c = q_reapable; 
     Thread* n = NULL;
     while(c != NULL){
         n = c->after;
         if( c->flags & THREAD_IS_COMPLETE){
             if( c->child_count <= c->dead_count){
                 _MyThreadListRemove(&q_reapable, c);
                 _MyThreadFree(c); 
             }            
         }
         c = n; 
     }

};

void MyThreadYield(){
    tcs("MyThreadYield()");
    _MyThreadPause(&q_ready);
};

/*
 *Queue the current thread to the queue specified
 */
void _MyThreadPause(Queue* q){
    tcs("_MyThreadPause(Queue * q)");
    _MyThreadPush(q, q_running);
    q_running->flags |= THREAD_YIELD_IND;
    swapcontext(&q_running->ctx, &q_running->return_ctx);
};

void _MyThreadPrint( Thread* t ){
    if( t == NULL ) t = q_running; 
    tcs("_MyThreadPrint( Thread* t )");
    info("Id     : %d", t->id);
    info("after Id: %d", (t->after)? t->after->id : -1);
    info("before Id: %d", (t->before)? t->before->id : -1);
    //info("ctx    : %s", (t->ctx)? "Yes" :"No");
    //info("ret_ctx: %s", (t->return_ctx)? "Yes" :"No");
};
void MyThreadExit(){
    NOOP();
};

void MyThreadInit(void(*f)(void *), void *args){
    tcs("MyThreadInit(void(*f)(void*), void* args)");
    MyThreadCreate(f, args);
    _MyThreadScheduler();
    info("Exiting");
    q_ready    = NULL;
    q_reapable = NULL;
    q_running  = NULL;
    q_blocked  = NULL;

};
void _MyThreadUnblock(Thread* t){
    tcs("_MyThreadUnblock(Thread* t)");
     _MyThreadListRemove(&q_blocked, t);
    _MyThreadPush(&q_ready, t);
};

void _MyThreadListRemove(Queue* from, Thread* t){
    tcs("_MyThreadListRemove(Queue* from, Queue* to, Thread* t)");
    Thread* aft = t->after; 
    Thread* bef = t->before;

    t->after  = NULL;
    t->before = NULL;

    if( aft == NULL && bef == NULL ) { //Only one in list;
        *from = NULL;

    } else if( aft == NULL ) { //end in the list; 
        bef->after  = NULL;

    } else if( bef == NULL ) {//first in the list;
        aft->before = NULL;
        *from = aft; 

    } else {  // middle; 
       aft->before = bef; 
       bef->after  = aft;
    }
};

Thread* _MyThreadGetCurrent(){
    return q_running; 
};

MySemaphore MySemaphoreInit(int initialValue){
    tcs("MySemaphoreInit(int initialValue)");
    Sem* s = malloc(sizeof(Sem));
    s->count = initialValue;
    s->q_blocked = NULL;
    return (MySemaphore) s; 
};

void MySemaphoreSignal(MySemaphore sem){
    tcs("MySemaphoreSignal(MySemaphore sem)");
    Sem* s = (Sem*) sem;
    if(s->count < 0 ) { 
       _MyThreadPush(&q_ready, _MyThreadPop(&s->q_blocked));
    }
    s->count++;
};

void MySemaphoreWait(MySemaphore sem){
    tcs("MySemaphoreWait(MySemaphore sem)");
    Sem* s = (Sem*) sem;
    s->count--;
    if(s->count <0){
        _MyThreadPause(&s->q_blocked);
    }
};

int MySemaphoreDestroy(MySemaphore sem){
    tcs("MySemaphoreDestroy(MySemaphore sem)");
    Sem* s = (Sem*) sem;
    if( s->q_blocked != NULL)
       return -1; 
    free(s); 
    return 0;          
};

#endif
