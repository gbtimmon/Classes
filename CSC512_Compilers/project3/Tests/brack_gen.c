int global[4];
int f0 ( int e, int f ){

    int local[14];

    local[3] = e;
    local[4] = f;

    local[7] = local[3] + local[4];
    local[6] = local[0 + local[7]];
    local[9] = global[0 + local[3]];
    local[11] = local[2];
    local[13] = 4 + 5;
    local[12] = local[0 + local[7]];
    local[10] = local[11] + local[12];
    local[8] = local[9] + local[10];
    local[5] = local[6] - local[8];
    local[0] = local[5];
};
