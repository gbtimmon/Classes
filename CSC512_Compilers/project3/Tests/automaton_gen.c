#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int state_0 ();

int state_1 ();

int state_2 ();

int state_3 ();

int getnextdigit (){

    int local[2];


    label_0: ;
    if(0==0) goto labal_2;
    goto label_1;
    label_2: ;
        print ( "Give me a number (-1 to quit): " );
        read ( local[0] );
        local[1] = - 1;
        if(local[1]<=local[0] )  goto label_3 ;
        goto label_4;
        label_3: ;
            goto label_1
        label_4: ;
        print ( "I need a number that's either 0 or 1.\n" );
    goto label_0 ;
    label_1: ;
    return local[0];
};
int state_0 (){

    int local[3];


    local[1] = getnextdigit ( );
    local[0] = local[1];
    local[2] = - 1;
    if(local[2]==local[0])  goto label_5;
    goto label_6;
    label_5: ;
        print ( "You gave me an even number of 0's.\n" );
        print ( "You gave me an even number of 1's.\n" );
        print ( "I therefore accept this input.\n" );
        return;
    label_6: ;
    if(0==local[0])  goto label_7;
    goto label_8;
    label_7: ;
        state_2 ( );
    label_8: ;
    if(1==local[0])  goto label_9;
    goto label_10;
    label_9: ;
        state_1 ( );
    label_10: ;
};
int state_1 (){

    int local[3];


    local[1] = getnextdigit ( );
    local[0] = local[1];
    local[2] = - 1;
    if(local[2]==local[0])  goto label_11;
    goto label_12;
    label_11: ;
        print ( "You gave me an even number of 0's.\n" );
        print ( "You gave me an odd number of 1's.\n" );
        print ( "I therefore reject this input.\n" );
        return;
    label_12: ;
    if(0==local[0])  goto label_13;
    goto label_14;
    label_13: ;
        state_3 ( );
    label_14: ;
    if(1==local[0])  goto label_15;
    goto label_16;
    label_15: ;
        state_0 ( );
    label_16: ;
};
int state_2 (){

    int local[3];


    local[1] = getnextdigit ( );
    local[0] = local[1];
    local[2] = - 1;
    if(local[2]==local[0])  goto label_17;
    goto label_18;
    label_17: ;
        print ( "You gave me an odd number of 0's.\n" );
        print ( "You gave me an even number of 1's.\n" );
        print ( "I therefore reject this input.\n" );
        return;
    label_18: ;
    if(0==local[0])  goto label_19;
    goto label_20;
    label_19: ;
        state_0 ( );
    label_20: ;
    if(1==local[0])  goto label_21;
    goto label_22;
    label_21: ;
        state_3 ( );
    label_22: ;
};
int state_3 (){

    int local[3];


    local[1] = getnextdigit ( );
    local[0] = local[1];
    local[2] = - 1;
    if(local[2]==local[0])  goto label_23;
    goto label_24;
    label_23: ;
        print ( "You gave me an odd number of 0's.\n" );
        print ( "You gave me an odd number of 1's.\n" );
        print ( "I therefore reject this input.\n" );
        return;
    label_24: ;
    if(0==local[0])  goto label_25;
    goto label_26;
    label_25: ;
        state_1 ( );
    label_26: ;
    if(1==local[0])  goto label_27;
    goto label_28;
    label_27: ;
        state_2 ( );
    label_28: ;
};
int main (){

    state_0 ( );
};
