#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int global[16];
int initialize_array (){

    int local[4];


    local[1] = 16;
    local[0] = 0;
    label_0: ;
    if(local[0]<local[1]) goto label_2;
    goto label_1;
    label_2: ;
        local[2] = - 1;
        global[0 + local[0]] = local[2];
        local[3] = local[0] + 1;
        local[0] = local[3];
    goto label_0;
    label_1: ;
};
int fib ( int val ){

    int local[9];

    local[0] = val;

    if(local[0]<2)  goto label_3;
    goto label_4;
    label_3: ;
        return 1;
    label_4: ;
    local[1] = global[0 + local[0]];
    local[2] = - 1;
    if(local[1]==local[2])  goto label_5;
    goto label_6;
    label_5: ;
        local[5] = local[0] - 1;
        local[4] = fib ( local[5] );
        local[7] = local[0] - 2;
        local[6] = fib ( local[7] );
        local[3] = local[4] + local[6];
        global[0 + local[0]] = local[3];
    label_6: ;
    local[8] = global[0 + local[0]];
    return local[8];
};
int main (){

    int local[4];


    local[1] = 16;
    initialize_array ( );
    local[0] = 0;
    print ( "The first few digits of the Fibonacci sequence are:\n" );
    label_7: ;
    if(local[0]<local[1]) goto label_9;
    goto label_8;
    label_9: ;
        local[2] = fib ( local[0] );
        write ( local[2] );
        local[3] = local[0] + 1;
        local[0] = local[3];
    goto label_7;
    label_8: ;
};
