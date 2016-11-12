#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
int main (){

    int local[5];

    read ( local[0] );
    local[3] = local[0] + 1;
    local[4] = local[0] / 2;
    local[2] = local[3] * local[4];
    local[1] = local[2];
    write ( local[1] );
};
