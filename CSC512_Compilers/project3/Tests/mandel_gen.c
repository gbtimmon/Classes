#include <stdio.h>
#define read(x) scanf("%d",&x)
#define write(x) printf("%d\n",x)
#define print(x) printf(x)
int square ( int x ){

    int local[4];

    local[0] = x;

    local[3] = local[0] * local[0];
    local[2] = local[3] + 500;
    local[1] = local[2] / 1000;
    return local[1];
};
int complex_abs_squared ( int real, int imag ){

    int local[5];

    local[0] = real;
    local[1] = imag;

    local[3] = square ( local[0] );
    local[4] = square ( local[1] );
    local[2] = local[3] + local[4];
    return local[2];
};
int check_for_bail ( int real, int imag ){

    int local[5];

    local[0] = real;
    local[1] = imag;

    local[2] = local[0] > 4000;
    local[3] = local[1] > 4000;
    if( local[2] || local[3])  goto label_0;
    goto label_1;
    label_0: ;
        return 0;
    label_1: ;
    local[4] = complex_abs_squared ( local[0] , local[1] );
    if( 1600 > local[4])  goto label_2;
    goto label_3;
    label_2: ;
        return 0;
    label_3: ;
    return 1;
};
int absval ( int x ){

    int local[3];

    local[0] = x;

    if( local[0] < 0)  goto label_4;
    goto label_5;
    label_4: ;
        local[2] = - 1;
        local[1] = local[2] * local[0];
        return local[1];
    label_5: ;
    return local[0];
};
int checkpixel ( int x, int y ){

    int local[20];

    local[5] = x;
    local[6] = y;

    local[0] = 0;
    local[1] = 0;
    local[3] = 0;
    local[4] = 16000;
    label_6: ;
    if( local[3] < 255) goto label_8;
    goto label_7;
    label_8: ;
        local[8] = square ( local[0] );
        local[10] = square ( local[1] );
        local[9] = local[10] + local[5];
        local[7] = local[8] - local[9];
        local[2] = local[7];
        local[15] = local[0] * local[1];
        local[14] = 2 * local[15];
        local[13] = local[14] + 500;
        local[12] = local[13] / 1000;
        local[11] = local[12] + local[6];
        local[1] = local[11];
        local[0] = local[2];
        local[17] = absval ( local[0] );
        local[18] = absval ( local[1] );
        local[16] = local[17] + local[18];
        if( local[16] > 5000)  goto label_9;
        goto label_10;
        label_9: ;
            return 0;
        label_10: ;
        local[19] = local[3] + 1;
        local[3] = local[19];
    goto label_6;
    label_7: ;
    return 1;
};
int main (){

    int local[8];

    local[1] = 950;
    label_11: ;
    local[3] = - 950;
    if( local[1] > local[3]) goto label_13;
    goto label_12;
    label_13: ;
        local[4] = - 2100;
        local[0] = local[4];
        label_14: ;
        if( local[0] < 1000) goto label_16;
        goto label_15;
        label_16: ;
            local[5] = checkpixel ( local[0] , local[1] );
            local[2] = local[5];
            if( 1 == local[2])  goto label_17;
            goto label_18;
            label_17: ;
                print ( "X" );
            label_18: ;
            if( 0 == local[2])  goto label_19;
            goto label_20;
            label_19: ;
                print ( " " );
            label_20: ;
            local[6] = local[0] + 40;
            local[0] = local[6];
        goto label_14;
        label_15: ;
        print ( "\n" );
        local[7] = local[1] - 50;
        local[1] = local[7];
    goto label_11;
    label_12: ;
};
