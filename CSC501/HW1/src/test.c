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
#include <_mythread.h>
#include <logmacros.h>
#include <stdarg.h>


#define NOOP() do {} while(0)
#define SIZE 100000
static int n[SIZE] = {};

void countTo(void* args){
    int j = 0; 
    printf("This is thread %d and I am about  to count to %d\n",_MyThreadGetCurrent()->id,j);
    for(; j<100; j++){
        NOOP();
    }

    printf("This is thread %d and I counted to %d\n",_MyThreadGetCurrent()->id,j);
};

void manyCounts(void* args){
    int *ip = (int*) args; 
    int i = *ip;
  
    int k=0;

    for(;k<ip[0];k++){
        printf("Making Thread %d of %d with count %d\n", k, ip[0], ip[1]);
        MyThreadCreate(countTo, &i);
    }
};

void Yielder (void *arg){
     int* i = (int*) arg;
     int j = 0; 
     while(j++ < 100){
        if( j % *i == 0 ) {
           printf("This is thread %d and I am about to yield\n",_MyThreadGetCurrent()->id);
           MyThreadYield();
        } else {
           printf("This is thread %d and I dont yeild because %d doesnt go into %d\n", _MyThreadGetCurrent()->id, *i, j);
        }
     }
}

void Yields(void *args){
    int* i = (int*) args;
    int j = 0; 

    while(j++ < *i) {
        MyThreadCreate(&Yielder, &n[j]);
    }
}

void Regression(void* args){
    int* ip = (int*) args; 
    int i = *ip;
       
    if( i > 0 ) { 
        MyThreadCreate(&Regression, &n[i-1]);
        MyThreadYield();
        MyThreadCreate(&Regression, &n[i-2]);
    } else {
        MyThreadCreate(&countTo, &n[50]);
    }
}

void Test(int i, char* title, void(*f)(void*), void* a) { 
     printf("###############################\n");
     printf(" Test %d : %s\n",i, title);
     printf("###############################\n");
     MyThreadInit(f, a);

};

void fib(void *in)
{
    int *n = (int *)in;	 	/* cast input parameter to an int * */

    if (*n == 0)
      /* pass */;			/* return 0; it already is zero */

  else if (*n == 1)
      /* pass */;			/* return 1; it already is one */

  else {
    int n1 = *n - 1;		/* child 1 param */
    int n2 = *n - 2;		/* child 2 param */

    MyThreadCreate(fib, (void*)&n1);
    MyThreadCreate(fib, (void*)&n2);
    MyThreadJoinAll();
    *n = n1 + n2;
    MyThreadExit();		// always call this at end
  }
}

void test1(void * l)
{
  MyThread T;
  int* kk = (int*) l;
  int n1 = *kk; 
  printf("test1 start %d\n", n1);

  int n2 = n1 -1 ;
  if (n1 > 0) {
    printf("test1 create\n");
    T = MyThreadCreate(test1, &n[n2]);
    if (n1 % 1 == 0)
      MyThreadYield();
    else if (n1 % 1 == 1)
      MyThreadJoin(T);
  }
  printf("test1 end\n");
  MyThreadExit();
}

void t1(void * wh)
{
  int* w = (int*) wh; 
  int who = *w;
  int i;

  printf("t%d start\n", who);
  for (i = 0; i < who; i++) {
    printf("t%d yield\n", who);
    MyThreadYield();
  }
  printf("t%d end\n", who);
  MyThreadExit();
}

void t0(void * dummy)
{
  int* i = (int*) dummy; 
  int ii = *i;
  puts("one");
  MyThreadCreate(t1,&n[ii]);
  puts("teat");
  t1(&n[ii*2]);
}

void RegJoinAll(void* args){
    int* ip = (int*) args;
    int i = *ip;

    if( i > 3 ) {
        MyThread T1 = MyThreadCreate(&RegJoinAll, &n[i-1]);
        MyThreadYield();
        MyThread T2 = MyThreadCreate(&RegJoinAll, &n[i-2]);
        MyThreadYield();
        MyThread T3 = MyThreadCreate(&RegJoinAll, &n[i-3]);
        if(i % 4 == 0) {
            printf("===Joining 1===\n");
            MyThreadJoin(T1);
            printf("===joinged 1===\n");
        } else if( i % 4 == 1) { 
            printf("===Joining 2===\n"); 
            MyThreadJoin(T2);
            printf("===joinged 2===\n");
        } else if ( i % 4 == 2) {
            printf("===Joinging 3===\n");
            MyThreadJoin(T3);
            printf("===joinged 3===\n");
        } else {
            printf("===Join All===\n");
            MyThreadJoinAll();
            printf("===joinged All===\n");
        }
    } else {
        MyThreadCreate(&countTo, &n[50]);
    }
}

static MySemaphore sem1;
static MySemaphore sem2;
void SemReader(void*);
void read(void);
void Sem1 ( void * args) { 


    int i = 0;
    sem1= MySemaphoreInit(0);
    sem2 = MySemaphoreInit(0);
    for(; i < 1; i++){
       MyThreadCreate( SemReader, &n[5]);
       MyThreadCreate( SemReader, &n[5]);
       MyThreadCreate( SemReader, &n[5]);
       
    }

    for(i=0; i <6; i++){
         MyThreadYield();
         MyThreadYield();
         MyThreadYield();
         MyThreadYield();
         if(i % 2 == 0 ) { 
             MySemaphoreSignal(sem1);
         } else {
             MySemaphoreSignal(sem2);
         }
    }

    MySemaphoreDestroy(sem1);
    MySemaphoreDestroy(sem2);
}

void SemReader ( void  * args ) { 
    MySemaphoreWait(sem1);
    read();
    MyThreadYield();
    MySemaphoreWait(sem2);
    read();
}

void read(void) { 
    puts("I am reading");
}

int main (){
    int i = 0 ;
    while (i++ < SIZE) {
        n[i] = i;
    }
    int input = 1; 
    while( input > 0 ) { 
        printf("Give me a test num :");
        if(!scanf("%d", &input)) input = 1;
        switch(input) {
         case 1: 
          Test(1, "basic thread", countTo, &n[5]);
          break;
         case 2:
          Test(2, "100 counters", manyCounts, &n[100]);
          break; 
         case 3:
          Test(3, "Yields", Yields, &n[30]);
          break;
         case 4:
          Test(4, "Regression", Regression, &n[12]);
          break; 
         case 5:
          i = n[15]; 
          printf("fib(%d) = ", i);
          Test(5, "Fib", fib, &i);
          printf("%d\n", i);
          break;
         case 6:
          Test(6, "Supplied Test1", test1, &n[1000]);
          break;
         case 7:
          Test(7, "Supplied Test2", t0, &n[50]);
          break;
         case 8:
          Test(8, "REgs and joins", RegJoinAll, &n[10]);
          break;
         case 9:
          Test(9, "Sema", Sem1, &n[50]);
         default:
          printf(" wut?\n");
        }
    }
    return 0;
};

#endif
