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
void f2 (void* args);
int a = 4;
int b = 7; 

void f0 (void* args) {
    puts("   This is thread 1 : %d", *((int*) args));
    MyThread m = MyThreadCreate(f2, &b);
    MyThreadCreate(f1, &a);
    MyThreadCreate(f1, &b);
    MyThreadCreate(f1, &a);
    MyThreadCreate(f1, &b);
    MyThreadCreate(f1, &a);
    MyThreadCreate(f1, &b);
    MyThreadJoinAll();
};

void f1 (void* args) {  
    puts("Thread %d : 1.)Start", Thread_getCurrent()->id);
    puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);
    puts("Thread %d : 4.)Exit", Thread_getCurrent()->id);
};

void f2(void* args){
    puts("Thread %d : 1.)Start", Thread_getCurrent()->id);
    puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);puts("Thread %d : 2.)Switch", Thread_getCurrent()->id);
    MyThreadYield();
    puts("Thread %d : 3.)Returning", Thread_getCurrent()->id);
};



void MyThreadInit( void(*f)(void*), void* args ) {  
    mcs("MyThreadInit(void(*f)(void*), void* args)");
    Thread_new(f, args);
    Thread_scheduler(); 
};

MyThread MyThreadCreate( void(*f)(void*), void* args ) { 
    mcs("MyThreadCreate(void(*start_funct)(void*), void* args)"); 
    return (MyThread) Thread_new(f, args);
}; 

void MyThreadYield( void ) { 
    mcs("MyThreadYeild( void )");
    Thread_yeild();
};

void MyThreadExit(void) { 
    mcs("MyThreadExit(void)");
    (void*) 0; 
};

int MyThreadJoin( MyThread waiton ) {
    mcs("MyThreadJoin( MyThread waiton )");
    Thread_join((Thread*) waiton );
    return 0;
};

void MyThreadJoinAll(void) {
    mcs("MyThreadJoinAll(void)");
    Thread_join_all(&ready_queue, running_thread);
};

MySemaphore MySemaphoreInit(int initialValue){
    return (MySemaphore) Sem_new(initialValue);
};

void MySemaphoreSignal(MySemaphore sem){
   
};
void MySemaphoreWait(MySemaphore sem){

};
int MySemaphoreDestroy(MySemaphore sem){

};

int k = 9; 
int main ( int argc, void* args){
    mcs("main ( int argc, void* args)");
    MyThreadInit(&f0, &k);
};

#endif
