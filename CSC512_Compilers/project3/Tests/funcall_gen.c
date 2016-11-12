#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int g (){

    return 1;
};
int f (){

    int local[2];

    local[1] = g ( );
    local[0] = local[1] + 1;
    return local[0];
};
int e (){

    int local[2];

    local[1] = f ( );
    local[0] = local[1] + 1;
    return local[0];
};
int d (){

    int local[2];

    local[1] = e ( );
    local[0] = local[1] + 1;
    return local[0];
};
int c (){

    int local[2];

    local[1] = d ( );
    local[0] = local[1] + 1;
    return local[0];
};
int b (){

    int local[2];

    local[1] = c ( );
    local[0] = local[1] + 1;
    return local[0];
};
int a (){

    int local[2];

    local[1] = b ( );
    local[0] = local[1] + 1;
    return local[0];
};
int main (){

    int local[2];

    local[1] = a ( );
    local[0] = local[1];
    print ( "I calculate the answer to be: " );
    write ( local[0] );
};
