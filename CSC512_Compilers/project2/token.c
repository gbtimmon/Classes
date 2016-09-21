#ifndef _TOKEN_C
#define _TOKEN_C

#define TOKEN_C_SIZE 4000

#define T_EOF      -1
#define T_UNDEFINED 0
#define T_LITERAL   1
#define T_OP        2
#define T_STRING    3 


typedef struct _token { 
    int type; 
    char d[ TOKEN_C_SIZE ];   
} * Token;

Token Token_new( ) { 
    Token n = malloc( sizeof( struct _token ) );
    n->type = T_UNDEFINED; 
    memset(n->d, 0, TOKEN_C_SIZE );
}

#endif
