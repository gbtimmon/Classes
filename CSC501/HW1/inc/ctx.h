#ifndef CTX_H
#define CTX_H 

#include <ucontext.h>

#ifdef _LP64
  #define STACK_SIZE 2097152+16384 /* large enough value for AMODE 64 */
#else
  #define STACK_SIZE 16384         /* AMODE 31 addressing */
#endif

typedef ucontext_t ctx_t; 
static ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link );
static ctx_t* _ctx_get(ctx_t* link);
static ctx_t* _ctx_link(ctx_t* from, ctx_t* to);

#endif
