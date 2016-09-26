#ifndef _TOKEN_C
#define _TOKEN_C

#include "token.h"
#include <stdio.h>

Token Token_new( token_t type, char * buffer ) { 

    Token n  = malloc( sizeof( struct _token ) );
    n->type  = type; 
    n->prev  = NULL;
    n->value = NULL;
    
    if( buffer ) n->value = malloc( sizeof( char ) * ( strlen(buffer) + 1 ) ) ;
    if( buffer ) strcpy( n->value, buffer ); 

    return n;
}

Token Token_free( Token n ){
    free(n->value); 
    free(n);
}

void Token_write( Token t, FILE * f ) { 
    char * s; 
    switch( t->type) {
        case T_ADD : s ="T_ADD"; break;
        case T_MULT : s ="T_MULT"; break;
        case T_RPAR : s ="T_RPAR"; break;
        case T_LPAR : s ="T_LPAR"; break;
        case T_RBRAC : s ="T_RBRAC"; break;
        case T_LBRAC : s ="T_LBRAC"; break;
        case T_COMMA : s ="T_COMMA"; break;
        case T_SEMI : s ="T_SEMI"; break;
        case T_OP : s ="T_OP"; break;
        case T_COMMENT : s ="T_COMMENT"; break;
        case T_META : s ="T_META"; break;
        case T_MCOMMENT : s ="T_MCOMMENT"; break;
        case T_VAR : s ="T_VAR"; break;
        case T_TYPE : s ="T_TYPE"; break;
        case T_KEYWORD : s ="T_KEYWORD"; break;
        case T_IDENT : s ="T_IDENT"; break;
        case T_NUMBER : s ="T_NUMBER"; break;
        case T_STRING : s ="T_STRING"; break;
        case T_EOF : s ="T_EOF"; break;
        case T_e : s ="T_e"; break;
        case S_DATA : s ="S_DATA"; break;
        case S_IDLIST : s ="S_IDLIST"; break;
        case S_IDLIST_ : s ="S_IDLIST_"; break;
        case S_ID : s ="S_ID"; break;
        case S_ID_ : s ="S_ID_"; break;
        case S_EXP : s ="S_EXP"; break;
        case S_EXP_ : s ="S_EXP_"; break;
        case S_TERM : s ="S_TERM"; break;
        case S_TERM_ : s ="S_TERM_"; break;
        case S_FACT : s ="S_FACT"; break;
        case S_FACT_ : s ="S_FACT_"; break;
    };
    fprintf( f, "TOKEN : %-10s [%s][%s]\n", s,isTerminal( t->type) ? "Y": "N", t->value );
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

void TokenStack_write( TokenStack s, FILE * f ){
    if( s->head != NULL ) TokenStack_writeRecurse( s->head, f); 
    fprintf(f, "\n");
};

void TokenStack_writeRecurse( Token t, FILE * f ){

    if( t->prev != NULL ) {
       TokenStack_writeRecurse( t->prev, f ); 
       fprintf(f, ", ");
    }
    
    
    char * s ; 
    switch( t->type) {
        case T_ADD : s ="T_ADD"; break;
        case T_MULT : s ="T_MULT"; break;
        case T_RPAR : s ="T_RPAR"; break;
        case T_LPAR : s ="T_LPAR"; break;
        case T_RBRAC : s ="T_RBRAC"; break;
        case T_LBRAC : s ="T_LBRAC"; break;
        case T_COMMA : s ="T_COMMA"; break;
        case T_SEMI : s ="T_SEMI"; break;
        case T_OP : s ="T_OP"; break;
        case T_COMMENT : s ="T_COMMENT"; break;
        case T_META : s ="T_META"; break;
        case T_MCOMMENT : s ="T_MCOMMENT"; break;
        case T_VAR : s ="T_VAR"; break;
        case T_TYPE : s ="T_TYPE"; break;
        case T_KEYWORD : s ="T_KEYWORD"; break;
        case T_IDENT : s ="T_IDENT"; break;
        case T_NUMBER : s ="T_NUMBER"; break;
        case T_STRING : s ="T_STRING"; break;
        case T_EOF : s ="T_EOF"; break;
        case T_e : s ="T_e"; break;
        case S_DATA : s ="S_DATA"; break;
        case S_IDLIST : s ="S_IDLIST"; break;
        case S_IDLIST_ : s ="S_IDLIST_"; break;
        case S_ID : s ="S_ID"; break;
        case S_ID_ : s ="S_ID_"; break;
        case S_EXP : s ="S_EXP"; break;
        case S_EXP_ : s ="S_EXP_"; break;
        case S_TERM : s ="S_TERM"; break;
        case S_TERM_ : s ="S_TERM_"; break;
        case S_FACT : s ="S_FACT"; break;
        case S_FACT_ : s ="S_FACT_"; break;
    };

    fprintf(f, "%s", s); 
};

    
#endif
