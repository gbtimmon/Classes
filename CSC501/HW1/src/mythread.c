/* 
 * myThread.c++
 *
 *  Created on: Jan 20, 2016
 *      Author: u236862
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>
#include "./mythread.h"

#ifndef MYTHREAD_C
#define MYTHREAD_C

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif

#define p(x) printf(x); printf("\n")


void func(int arg) { printf( "function called with value %d\nprocess will exit when function returns\n",arg); };
void f1 (void* args) { int i;  for (i = 0 ; i < 100; i++) { printf("[0.)%d]\n",(int*)args);}}; 
void f2 (void* args) { int i;  for (i = 58; i < 127; i++){ printf("[1.)%d]\n",(int*)args);}}; 

typedef ucontext_t ctx_t; 

static int    	threadCount   = 0; 
static int      initialized   = 0; 
static ctx_t*   ctx_list_head; 
static ctx_t*   ctx_list_tail;

void _myThread_init(){
    if( !initialized ) { 
       p("intializing") ;       
       initialized = 1; 
       ctx_list_head = (ctx_t*) malloc(sizeof(ctx_t));      
       getcontext(ctx_list_head);
       ctx_list_tail = ctx_list_head; 
    }
}

MyThread MyThreadCreate(void(*start_funct)(void *), void *args){

    p("Thread init");
    _myThread_init();

    ctx_t* t = (ctx_t*) malloc(sizeof(ctx_t));
 
    getcontext(t); 

    t->uc_stack.ss_sp   = (char *) malloc(STACK_SIZE);
    t->uc_stack.ss_size = STACK_SIZE; 
    t->uc_flags         = 0; 
   
    makecontext(t, (void(*)()) start_funct, 1, args);

    ctx_list_head->uc_link = t;
    ctx_list_head = t;

    return (MyThread) t; 
}

void MyThreadYeild(void) { 
     
}




int main() { 
        MyThread mt1 = MyThreadCreate( &f1, (void *) 0);
        MyThread mt2 = MyThreadCreate( &f2, (void *) 0); 
        ctx cur; 
        swapcontext(&mt2,        
    
};
#endif

