/*
 * myThread.c++
 *
 *  Created on: Jan 20, 2016
 *      Author: u236862
 */
#ifndef MYTHREAD_C
#define MYTHREAD_C

#define INFO

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>
#include <mythread.h>
#include <thread.h>
#include <logmacros.h>




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
    printf("Thread %d : 1.)Start\n", running_thread->id);
    printf("Thread %d : 2.)Switch\n", running_thread->id);
    MyThreadYield();
    printf("Thread %d : 3.)Returning\n", running_thread->id);
    printf("Thread %d : 4.)Exit\n", running_thread->id);
};

void MyThreadInit( void(*f)(void*), void* args ) {  
    debug("> void     MyThreadInit(void(*f)(void*), void* args)");
    Thread_push( &ready_queue, Thread_new("First", thread_count++, f, args) );
    Thread_scheduler(&ready_queue, &running_thread); 
};

MyThread MyThreadCreate( void(*f)(void*), void* args ) { 
    debug("> MyThread MyThreadCreate(void(*start_funct)(void*), void* args)"); 
    Thread_push( &ready_queue,  Thread_new("MTC", thread_count++, f, args) ); 
}; 

void MyThreadYield( void ) { 
    debug("> void    MyThreadYeild( void )");
    Thread* me = running_thread; 
    Thread_push( &ready_queue, me);
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
