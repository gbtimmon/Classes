#ifndef _TOKEN_C
#define _TOKEN_C

#include "token.h"
#include "transform.h"
#include "symbol.h"
#include <stdio.h>
#include "token.gen.c"

#define NAME(n) tokenName[ (int) n->type ]

int global_id = 1; 

int _break_=0; 
#define BREAK() {printf("BREAK %d\n", _break_++ ); fflush( NULL ); }
#define RBREAK() { _break_ = 0; BREAK() }
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
    n->scope   = NULL;   // a symbol table if one exists here. 
    
    if( buffer ) n->value = malloc( sizeof( char ) * ( strlen(buffer) + 1 ) ) ;
    if( buffer ) strcpy( n->value, buffer ); 

    return n;
};

char * Token_toString( Token n ) {
    int len = strlen( NAME(n) ); 

    int len3 = ( isTerminal( n->type ) ) ? strlen(n->value)+5 : 0 ;

    char temp[250]; 
    if( isTerminal( n->type ) )
        sprintf( temp, " \"%s\" ", n->value ); 
    else 
        sprintf( temp, "" ); 

 
    char * fmt = "<%3d:%s %s>";
    char * str = malloc( sizeof( char ) * (15+len+len3) ); 
    sprintf( str, fmt, n->id, NAME(n), temp );
    return str; 
};

void Token_free( Token n ){
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

Token Token_findChild( Token p, token_t type ) {

    Token c = p->child; 
    while( c != NULL ) {
        if( c->type == type )
           return c; 
        c = c->peer; 
    }
    return NULL;
}; 

int Token_countChild( Token t, token_t  type ) {

    if( t == NULL ) return 0; 

    Token c = t->child; 

    int i = 0; 
    while( c != NULL ) {
        if( type == -1 ) {
            i++; 
        } else if ( c->type == type ) {
            i++; 
        }
        c = c->peer; 
    }
    return i;
}; 

void Token_collapse( Token t ) {
     Token start = t->lpeer; 
     Token end   = t->peer; 
     Token c     = t->child; 

     //if im empty just remove. 
     if( c == NULL ){
         Token_remove( t ); 
         Token_free( t ); 
         return;
     }

     c->lpeer    = start; 
     if( c->lpeer != NULL ) 
         c->lpeer->peer = c; 
     else 
         t->parent->child = c; 

     c->parent = t->parent; 
     while( c->peer != NULL ) {
         c = c->peer; 
         c->parent = t->parent; 
     }

     c->peer = end; 
     if( c->peer != NULL )
         c->peer->lpeer = c; 

     Token_free( t ); 
     return; 
};

Token Token_remove( Token t ) {

    //connect my left to my right
    if( t->lpeer != NULL ) t->lpeer->peer = t->peer; 
    //connect my right to my left
    if( t->peer  != NULL ) t->peer->lpeer = t->lpeer; 
    //if I am first in the list, move the parent pointer to the next. 
    if( t->lpeer  == NULL && t->parent != NULL ) t->parent->child = t->peer;

    //forget where I was. 
    t->parent = NULL; 
    t->peer   = NULL;
    t->lpeer  = NULL;

    return t; 
};

Token Token_replace( Token dest, Token src ) { 
    Token_remove( src ); 
    
    //My neighbors are what used to be my dest neighbors
    src->peer  = dest->peer; 
    src->lpeer = dest->lpeer;  

    //Let my new neighbors know Im here.
    if( src->peer  != NULL ) src->peer->lpeer = src; 
    if( src->lpeer != NULL ) src->lpeer->peer = src; 
    src->parent = dest->parent;

    //If the dest is the first in the list, let the parent know. 
    if( dest->parent != NULL && dest->parent->child->id == dest->id )
       dest->parent->child = src; 

    dest->peer   = NULL;  
    dest->lpeer  = NULL;
    dest->parent = NULL;
    return dest; 
}

Token Token_join( Token dest, Token src ) { 


    Token_remove( src ); 

    //For each child of from, insert it into the dest
    while( src->child != NULL ) { 
        Token c = Token_remove( src->child ); 
        Token_prependChild( dest, c ); 
    }

    Token_free(src); 
};


void Token_printTree( Token h ) {

   char * myself = Token_toString( h ); 

       
   if( h->child == NULL && !isTerminal( h->type) ){
       printf( "%s ->", myself ); 
       printf(" NULL\n"); 
   } else if( !isTerminal( h->type ) && h->type != 99999 ) {
       printf( "%s ->", myself ); 
       Token c = h->child; 
       while( c != NULL ){
           char * cstr = Token_toString( c ); 
           printf( " %s", cstr );  
           free( cstr ); 
           c = c->peer; 
       }
       printf("\n"); 

       if( h->scope != NULL )
           SymbolTable_write( h->scope ); 
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
