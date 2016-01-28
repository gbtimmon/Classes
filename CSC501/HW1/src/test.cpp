#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <errno.h>

 #ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
 #else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
 #endif

#define p(x) printf(x); printf("\n")

void func(int);
void func2(int) ;  
typedef ucontext_t ctx_t;

static ctx_t* ctx_queue_in  = 0; 
static ctx_t* ctx_queue_out = 0; 

void schedule(ctx_t* ctx);
void MakeThread(void(*func)(void*), void* args);
void func(int arg);
void func2(int arg);

int main(void) {

  int  value = 1;

  if ( ctx_queue_in == 0 ) { 
     p("not init!"); 
  } 

  ctx_queue_in = (ctx_t*) malloc(sizeof(ctx_t)); 
  ctx_queue_out = ctx_queue_in; 
  getcontext(ctx_queue_in); 

  if ( ctx_queue_in == 0 ) {
     printf("not init!");
  }
  printf("testing stuff is fun\n"); 
  
  ctx_t* thread = (ctx_t*) malloc(sizeof(ctx_t)); 
  getcontext(thread);
  thread->uc_link = ctx_queue_in;
  thread->uc_stack.ss_sp = (char *) malloc(STACK_SIZE);
  thread->uc_stack.ss_size = STACK_SIZE;
  thread->uc_stack.ss_flags = 0;
  p("lemon");
  makecontext(thread, (void(*)()) func, 1, 2); 
  p("lime"); 
  swapcontext(ctx_queue_in, thread); 
  p("cat");
  printf("after\n");   
}

void MakeThread(void(*func)(void*), void* args) { 
     ctx_t* newThread = (ctx_t*) malloc(sizeof(ctx_t));
     getcontext(newThread);

     
}

void func(int arg) {

  printf("function called with value %d\n",arg);
  printf("process will exit when function returns\n");
  return;

}

void func2(int arg) {
  printf("This is the second thread value is %d\n");
  return; 
}
