#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
int recursionsum ( int n ){

    int local[4];

    local[0] = n;

    if( local[0] == 0)  goto label_0;
    goto label_1;
    label_0: ;
        return 0;
    label_1: ;
    local[3] = local[0] - 1;
    local[2] = recursionsum ( local[3] );
    local[1] = local[0] + local[2];
    return local[1];
};
int main (){

    int local[2];

    read ( local[0] );
    local[1] = recursionsum ( local[0] );
    write ( local[1] );
};
