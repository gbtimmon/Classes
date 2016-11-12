#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int recursedigit ( int n ){

    int local[6];

    local[1] = n;

    if(0==local[1])  goto label_0;
    goto label_1;
    label_0: ;
        return;
    label_1: ;
    local[0] = 0;
    local[4] = local[1] / 2;
    local[3] = local[4] * 2;
    local[2] = local[1] - local[3];
    if(0!=local[2])  goto label_2;
    goto label_3;
    label_2: ;
        local[0] = 1;
    label_3: ;
    local[5] = local[1] / 2;
    recursedigit ( local[5] );
    if(0==local[0])  goto label_4;
    goto label_5;
    label_4: ;
        print ( "0" );
    label_5: ;
    if(1==local[0])  goto label_6;
    goto label_7;
    label_6: ;
        print ( "1" );
    label_7: ;
};
int main (){

    int local[1];

    local[0] = 0;
    label_8: ;
    if(0>=local[0]) goto label_10;
    goto label_9;
    label_10: ;
        print ( "Give me a number: " );
        read ( local[0] );
        if(0>=local[0])  goto label_11;
        goto label_12;
        label_11: ;
            print ( "I need a positive integer.\n" );
        label_12: ;
    goto label_8;
    label_9: ;
    print ( "The binary representation of: " );
    write ( local[0] );
    print ( "is: " );
    recursedigit ( local[0] );
    print ( "\n\n" );
};
