#ifndef _TOKEN_C
#define _TOKEN_C

#include "token.h"
#include <stdio.h>

Token Token_new( token_t type, char * buffer ) { 

    Token n  = malloc( sizeof( struct _token ) );
    n->type  = type; 
    n->value = malloc( sizeof( char ) * ( strlen(buffer) + 1 ) ) ;
    n->prev  = NULL;

    strcpy( n->value, buffer ); 
    return n;
}

Token Token_free( Token n ){
    free(n->value); 
    free(n);
}

void Token_write( Token t, FILE * f ) { 
    char * c; 
    switch( t->type) {
        case T_KEYWORD  : c = "T_KEYWORD";  break;
        case T_EOF      : c = "T_EOF";      break;
        case T_EMPTY    : c = "T_EMPTY";    break;
        case T_IDENT    : c = "T_IDENT";    break;
        case T_META     : c = "T_META";     break;
        case T_STRING   : c = "T_STRING";   break;
        case T_MCOMMENT : c = "T_MCOMMENT"; break;
        case T_COMMENT  : c = "T_COMMENT";  break;
        case T_OP       : c = "T_OP";       break;
        case T_NUMBER   : c = "T_NUMBER";   break;
    };
    fprintf( f, "TOKEN : %-10s[%s]\n", c, t->value );
};
    

TokenStack TokenStack_new ( void ) {
    TokenStack s = malloc( sizeof (struct _token_stack ) );
    s->head = NULL;
};

void TokenStack_free( TokenStack s ){
    while( s->head != NULL ) {
        Token t = s->head;
        s->head = t->prev;
        Token_free( t );
    };
    free( s );
};

void TokenStack_push( TokenStack s, Token t ) {
    t->prev = s->head;
    s->head = t;
};

Token TokenStack_pop ( TokenStack s ) {
    if( s->head == NULL ) 
        return NULL;

    Token t = s->head;
    s->head = t->prev;
    t->prev = NULL;
    return t;
};
#endif
