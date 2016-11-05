#ifndef _TOKEN_C
#define _TOKEN_C

#include "token.h"
#include <stdio.h>
#include "token.gen.c"

Token Token_new( token_t type, char * buffer ) { 

    Token n  = malloc( sizeof( struct _token ) );
    n->type  = type; 
    n->child = NULL;
    n->peer  = NULL;
    n->value = NULL;
    
    if( buffer ) n->value = malloc( sizeof( char ) * ( strlen(buffer) + 1 ) ) ;
    if( buffer ) strcpy( n->value, buffer ); 

    return n;
}

void Token_free( Token n ){
    free(n->value); 
    free(n);
}

void Token_write( Token t, FILE * f ) { 
    fprintf( f, "TOKEN : %-10s [%s][%s]\n", tokenName[ (int) t->type ], isTerminal( t->type) ? "Y": "N", t->value );
};

int isTerminal( token_t t ) {
    return (t < TERM_COUNT);
};

int isSkip( token_t t ) { 
    return ( t == T_COMMENT || t == T_META || t == T_MCOMMENT );
};

TokenStack TokenStack_new ( void ) {
    TokenStack s = malloc( sizeof (struct _token_stack ) );
    s->head = NULL;
    return s;
};

void TokenStack_free( TokenStack s ){
    TokenStackEle e = s->head;
    while( e != NULL ) {
        TokenStackEle n = e->next;
        Token_free( e );
        e = n;
    };
    free( s );
};

void TokenStack_push( TokenStack s, Token t ) {

    TokenStackEle n = malloc( sizeof( struct _token_stack_ele ) );
    n->ele = t;
    n->next = s->head;
    s->head = n;
};

Token TokenStack_pop ( TokenStack s ) {
    if( s->head == NULL ) 
        return NULL;

    TokenStackEle e = s->head;
    Token t         = e->ele;
    s->head         = e->next;
    free( e );
    return t;
};

void TokenStack_write( TokenStack s, FILE * f ){
    if( s->head != NULL ) TokenStack_writeRecurse( s->head, f); 
    fprintf(f, "\n");
};

void TokenStack_writeRecurse( TokenStackEle t, FILE * f ){

    if( t->next != NULL ) {
       TokenStack_writeRecurse( t->next, f ); 
       fprintf(f, ", ");
    }
    fprintf(f, "%s", tokenName[ (int) t->ele->type ] ); 
};
   
#endif
