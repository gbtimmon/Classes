#ifndef RANDOM_C
#define RANDOM_C

#define SEED 12563128356
#include <stdlib.h>

static int init = 0;
int random( ) {  
    if(init == 0){
        srand(SEED);
        init = 1; 
    }
    return rand(); 
}
#endif
