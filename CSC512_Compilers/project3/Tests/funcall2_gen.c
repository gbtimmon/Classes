#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int c (){

    return 1;
};
int b (){

    return 2;
};
int a (){

    return 3;
};
int foo ( int a, int b, int c ){

    int local[7];

    local[0] = a;
    local[1] = b;
    local[2] = c;

    local[5] = local[0] * 3;
    local[6] = local[1] * 2;
    local[4] = local[5] + local[6];
    local[3] = local[4] + local[2];
    return local[3];
};
int main (){

    int local[5];

    local[2] = a ( );
    local[3] = b ( );
    local[4] = c ( );
    local[1] = foo ( local[2] , local[3] , local[4] );
    local[0] = local[1];
    print ( "I calculate the answer to be: " );
    write ( local[0] );
};
