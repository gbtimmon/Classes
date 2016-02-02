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
    int* ip = (int*) args; 
    int  i  = *ip;
 
    int j = 0; 
    printf("This is thread %d and I am about  to count to %d\n",_MyThreadGetCurrent()->id,j);
    for(; j<i; j++){
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

void Test(int i, char* title, void(*f)(void*), void* a) { 
     printf("###############################\n");
     printf(" Test %d : %s\n",i, title);
     printf("###############################\n");
     MyThreadInit(f, a);

};

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
          Test(2, "100 counters", manyCounts, &n[2]);
          break; 
         case 3:
          Test(3, "Yields", Yields, &n[30]);
          break;
         default:
          printf(" wut?\n");
        }
    }
    return 0;
};

#endif
