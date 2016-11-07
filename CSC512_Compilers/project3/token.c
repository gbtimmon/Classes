#ifndef _TOKEN_C
#define _TOKEN_C

#include "token.h"
#include "transform.h"
#include "symbol.h"
#include <stdio.h>
#include "token.gen.c"

#define NAME(n) tokenName[ (int) n->type ]

int global_id = 1; 

int isTerminal( token_t t ) {
    return (t < TERM_COUNT);
};

Token Token_new( token_t type, char * buffer ) { 

    Token n    = malloc( sizeof( struct _token ) );
    n->id      = global_id++; 
    n->type    = type; 

    n->value   = NULL;   // String data from the source. 
    n->child   = NULL;   // pointer to the tokens I generated 
    n->parent  = NULL;   // pointer to the token that generated me
    n->peer    = NULL;   // pointer to my peer ( I am part of a linked list whose head is pointed to by my parent )
    n->lpeer   = NULL;   // pointer to me left peer ( its a doublely linked list ). 
    n->data    = NULL;   // pointer to a data struct. 
    n->scope   = NULL;   // a symbol table if one exists here. 
    
    if( buffer ) n->value = malloc( sizeof( char ) * ( strlen(buffer) + 1 ) ) ;
    if( buffer ) strcpy( n->value, buffer ); 

    return n;
};

char * Token_toString( Token n ) {
    int len = strlen( NAME(n) ); 

    char * pstr = getPayloadString( n );
    int len2 = strlen( pstr );   
    
    int len3 = ( isTerminal( n->type ) ) ? strlen(n->value)+5 : 0 ;

    char temp[250]; 
    if( isTerminal( n->type ) )
        sprintf( temp, " \"%s\" ", n->value ); 
    else 
        sprintf( temp, "" ); 

 
    char * fmt = "<%3d:%s [%s]%s>";
    char * str = malloc( sizeof( char ) * (15+len+len2+len3) ); 
    sprintf( str, fmt, n->id, NAME(n), pstr, temp );

    free( pstr ); 
    return str; 
};

void Token_free( Token n ){
    free(n->data); 
    free(n->value); 
    if( n->scope != NULL ) SymbolTable_free( n->scope ); 
    free(n);
};

void Token_appendChild( Token parent, Token lchild ) {

    lchild->parent = parent;

    if( parent->child == NULL ) {
        parent->child  = lchild;
        lchild->peer   = NULL;
        lchild->lpeer  = NULL;
    } else {
        Token rchild = parent->child; 
        parent->child = lchild; 
        lchild->peer  = rchild; 
        lchild->lpeer = NULL;
        rchild->lpeer = lchild; 
    }
};

void Token_prependChild( Token b, Token c ) { 

    c->parent = b; 
   
    if( b->child == NULL ) {
        b->child = c; 
        c->peer  = NULL;
        c->lpeer = NULL;
    } else {
        Token last = b->child; 
        while( last->peer != NULL ) last = last->peer; 
        last->peer = c; 
        c->peer    = NULL;
        c->lpeer   = last; 
    }

};

void Token_printTree( Token h ) {

   char * myself = Token_toString( h ); 

       
   if( h->child == NULL && !isTerminal( h->type) ){
       printf( "%s ->", myself ); 
       printf(" NULL\n"); 
   } else if( !isTerminal( h->type ) ) {
       printf( "%s ->", myself ); 
       Token c = h->child; 
       while( c != NULL ){
           char * cstr = Token_toString( c ); 
           printf( " %s", cstr );  
           free( cstr ); 
           c = c->peer; 
       }
       printf("\n"); 
   }
   free( myself ); 
   if( h->child != NULL ) Token_printTree( h->child ); 
   if( h->peer  != NULL ) Token_printTree( h->peer  ); 
}

void Token_write( Token t, FILE * f ) { 
    fprintf( f, "TOKEN : %-10s [%s][%s]\n", tokenName[ (int) t->type ], isTerminal( t->type) ? "Y": "N", t->value );
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
    TokenStackElement e = s->head;
    while( e != NULL ) {
        TokenStackElement p = e->prev;
        Token_free( e->token );
        free( e );
        e = p;
    }
    free( s );
};

void TokenStack_push( TokenStack s, Token t ) {
    TokenStackElement e = malloc( sizeof( struct _tokenStackElement ) );
    e->token = t; 
    e->prev  = s->head;
    s->head  = e;
};

Token TokenStack_pop ( TokenStack s ) {
    if( s->head == NULL ) 
        return NULL;

    TokenStackElement e = s->head;
    Token t = e->token; 
    s->head = e->prev;
    e->prev = NULL;
    e->token = NULL;
    free( e );
    return t;
  
};

void TokenStack_write( TokenStack s, FILE * f ){
    if( s->head != NULL ) TokenStack_writeRecurse( s->head, f); 
    fprintf(f, "\n");
};

void TokenStack_writeRecurse( TokenStackElement t, FILE * f ){

    if( t->prev != NULL ) {
       TokenStack_writeRecurse( t->prev, f ); 
       fprintf(f, ", ");
    }
    fprintf(f, "%s", tokenName[ (int) t->token->type ] ); 
};
   
#endif
