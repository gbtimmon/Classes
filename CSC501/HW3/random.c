#ifndef RANDOM_C
#define RANDOM_C

#define SEED 1128356
#include <stdlib.h>

static int init = 0;
int nlrandom()  {  
    if(init == 0){
        srand(SEED);
        init = 1; 
    }
    return rand(); 
}

int idrandom( int id ) {
    if( init < 2 ) {
       srand( id );
       init = 2;
    }
    return rand();
}
#endif
