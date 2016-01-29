/*
 *  * myThread.c++
 *   *
 *    *  Created on: Jan 20, 2016
 *     *      Author: u236862
 *      */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>
#include "mythread.h"
//#include "ctx.h"

#ifndef MYTHREAD_C
#define MYTHREAD_C

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif

#define p(x) printf(x); printf("\n")

#define DEBUG 1

void func(int arg) { printf( "function called with value %d\nprocess will exit when function returns\n",arg); };
void f1 (void* args) { int i;  for (i = 0 ; i < 100; i++) { printf("[0.)%d]\n",(int*)args);}};
void f2 (void* args) { int i;  for (i = 58; i < 127; i++){ printf("[1.)%d]\n",(int*)args);}};

static int      threadCount   = 0;
static int      initialized   = 0;
static ctx_t*   ctx_list_head = (ctx_t*) 0; 
//static ctx_t*   ctx_list_tail = (ctx_t*) 0; 

static void f3 (void* _i ) {

    #ifdef DEBUG
       p("CALL : static f3 (void* _i )");
    #endif

    int  j; 
    int* i = (int*) _i; 
    for( j = 0; j < *i; j++) printf("This is a int : %d\n", j); 
}

/*
static ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link ) {

    #ifdef DEBUG
        p("CALL : static ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link )");
    #endif

    ctx_t* n            = (ctx_t*) malloc(sizeof(ctx_t));
    getcontext(n);
    n->uc_stack.ss_sp   = (char *) malloc(STACK_SIZE);
    n->uc_stack.ss_size = STACK_SIZE;
    n->uc_flags         = 0;

    if(link) {
        n->uc_link = link;
    }

    if(f) {
        makecontext( n, (void(*)()) f, 1, args);
    }

    return n;
};

static ctx_t* _ctx_get(ctx_t* link){

    #ifdef DEBUG
       p("CALL : static ctx_t* _ctx_get(ctx_t* link)");
    #endif

    return _ctx_make(NULL, NULL, link);
};


static ctx_t* _ctx_link(ctx_t* from, ctx_t* to){

    #ifdef DEBUG
        p("CALL : static ctx_t* _ctx_link(ctx_t* from, ctx_t* to)");
    #endif
    from->uc_link = to;
};
tatic ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link ) {

    #ifdef DEBUG
        p("CALL : static ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link )");
    #endif

    ctx_t* n            = (ctx_t*) malloc(sizeof(ctx_t));
    getcontext(n);
    n->uc_stack.ss_sp   = (char *) malloc(STACK_SIZE);
    n->uc_stack.ss_size = STACK_SIZE;
    n->uc_flags         = 0;
 
    if(link) {
        n->uc_link = link; 
    }
 
    if(f) {
        makecontext( n, (void(*)()) f, 1, args); 
    }
 
    return n; 
};

static ctx_t* _ctx_get(ctx_t* link){

    #ifdef DEBUG
       p("CALL : static ctx_t* _ctx_get(ctx_t* link)");
    #endif

    return _ctx_make(NULL, NULL, link);
};


static ctx_t* _ctx_link(ctx_t* from, ctx_t* to){

    #ifdef DEBUG
        p("CALL : static ctx_t* _ctx_link(ctx_t* from, ctx_t* to)");
    #endif
    from->uc_link = to;    
};
*/

void MyThreadInit(void(*f)(void*), void* args) { 

    #ifdef DEBUG
        p("CALL : void MyThreadInit(void(*f)(void*), void* args)");
    #endif

    if(ctx_list_head != 0 ) {
        MyThreadCreate(f, args);
        fprintf(stderr, "Warning : MyThreadInit called from already initialized ThreadManager\n"); 
        return; 
    }    

    p("im trying");
    ctx_t* t_main = _ctx_get ( (ctx_t*) 0 ); 
    ctx_t* t_frst = _ctx_make( f, args, t_main); 
    ctx_list_head = t_main; 
    swapcontext( t_main, t_frst);
    
    p("OhGodTheAfterMath\n");
}

MyThread MyThreadCreate(void(*f)(void*), void* args) { 
    
    #ifdef DEBUG
        p("CALL : MyThread MyThreadCreate(void(*start_funct)(void*), void* args)");
    #endif    

    ctx_t* t_new = _ctx_make((void(*)()) f, args, (ctx_t*) 0 ); 
          
}

int main ( int argc, void* args){

    #ifdef DEBUG
        p("CALL : int main ( int argc, void* args)");
    #endif
    
    p("hello");
    int i = 8;
    MyThreadInit( f3, (void*) &i); 
};

#endif
