#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
int main (){

    int local[4];

    read ( local[0] );
    local[1] = 0;
    label_0: ;
    if( local[0] > 0) goto label_2;
    goto label_1;
    label_2: ;
        local[2] = local[1] + local[0];
        local[1] = local[2];
        local[3] = local[0] - 1;
        local[0] = local[3];
    goto label_0;
    label_1: ;
    write ( local[1] );
};
