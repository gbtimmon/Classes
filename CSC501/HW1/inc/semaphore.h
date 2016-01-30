#ifndef SEMAPHORE_C
#define SEMAPHORE_C

#include <semaphore.h>

typedef struct Sem {
    int     count;
    Thread* q_blocked;

} Sem;

Sem Sem_new(int i){
    Sem* s = malloc(sizeof(Sem));
    s->count = i;
    s->q_blocked


#endif
