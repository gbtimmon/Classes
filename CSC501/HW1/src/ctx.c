#ifndef CTX_C
#define CTX_C

#include <ucontext.h>
#include <stdlib.h>
#include <ctx.h>


ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link ) {

    #ifdef DEBUG
        p("CALL :  ctx_t* _ctx_make(void(*f)(void*), void* args, ctx_t* link )");
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

ctx_t* _ctx_get(ctx_t* link){

    #ifdef DEBUG
       p("CALL :  ctx_t* _ctx_get(ctx_t* link)");
    #endif

    return _ctx_make(NULL, NULL, link);
};


ctx_t* _ctx_link(ctx_t* from, ctx_t* to){

    #ifdef DEBUG
        p("CALL :  ctx_t* _ctx_link(ctx_t* from, ctx_t* to)");
    #endif
    from->uc_link = to;
};
#endif
