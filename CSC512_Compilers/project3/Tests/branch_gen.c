#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
int main (){

    int local[2];

    read ( local[0] );
    read ( local[1] );
    if( local[0] >= local[1])  goto label_0;
    goto label_1;
    label_0: ;
        write ( local[0] );
    label_1: ;
    if( local[1] > local[0])  goto label_2;
    goto label_3;
    label_2: ;
        write ( local[1] );
    label_3: ;
};
