#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int global[8];
int populate_arrays (){


    global[0] = 0;
    global[1] = 1;
    global[2] = 1;
    global[3] = 2;
    global[4] = 3;
    global[5] = 5;
    global[6] = 8;
    global[7] = 13;
};
int main (){

    int local[4];


    populate_arrays ( );
    local[0] = 0;
    local[1] = 8;
    print ( "The first few digits of the Fibonacci sequence are:\n" );
    label_0: ;
    if(local[0]<local[1]) goto label_2;
    goto label_1;
    label_2: ;
        local[2] = global[0 + local[0]];
        write ( local[2] );
        local[3] = local[0] + 1;
        local[0] = local[3];
    goto label_0;
    label_1: ;
};
