int a,b,c; 

int f() {
    return a + b; 
}

int d( int e, int f ){
    int g; 

    f();
    g = a + b * c - e * f[a];
    return g; 
}
