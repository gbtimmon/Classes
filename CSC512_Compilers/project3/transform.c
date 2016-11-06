#ifndef _TRANSFORM_C_
#define _TRANSFORM_C_

#include "transform.h"
#include "assert.h"

int _break_; 
#define BREAK() {printf("BREAK %d\n", _break_++);fflush(NULL);}

/** Recode the IR Tree formed by the parser. 
    Do a depth / last item first scan and recode as we go. 
    This way subtrees are recoded before we recode a node. 
     **/



char * getPayloadString ( Token t ) {
    switch( t->type ) {
        case T_XDATA : {
            char * fmt = "Value Count %3d";
            char * str = malloc( sizeof( char ) * strlen( fmt ) + 5 ); 
            struct xData * data = (struct xData * ) t->data; 
            sprintf( str, fmt, data->varCount ); 
            return str; 
        }
        default :{
            char * str = malloc( sizeof( char ) ); 
            sprintf( str, "%s", "" ); 
            return str; 
        }
    }
};


void doTransform( Token t ) {

    switch( t->type ) {
        case S_TYPE : { 
            Token c   = t->child;
            c->type   = T_XTYPE;
            c->peer   = t->peer; 
            c->lpeer  = t->lpeer; 
            c->parent = t->parent;
            if( t->parent->child->lpeer == NULL ) //Im first in the list
               t->parent->child = c; 
            return; 
        }
        case S_IDLIST : {
     
            // I dont need the comma.  
            t->child = t->child->peer; 
            Token_free( t->child->lpeer );
            t->child->lpeer = NULL;
            
            // collapse upwards since I dont need depth. 
            Token c    = t->child; 
            Token last;
            while( c != NULL ) {
                last = c; 
                c->parent = t->parent; 
                c = c->peer; 
            }

            Token first = t->child; 
   
            first->lpeer   = t->lpeer; 
            t->lpeer->peer = first; 
   
            last->peer = t->peer; 
            if( t->peer != NULL )
                t->peer->lpeer = last; 
                 
            return; 
        }
        case S_START_A : {
            //Lets roll the S_START_A into the start list. 
            Token parent  = t->parent;
            Token left    = t->lpeer; 
            Token right   = t->child; 
            left->peer    = right;
            right->lpeer  = left; 
    
            BREAK();

            Token c = t->child;
            while( c != NULL ) {
                c->parent = parent; 
                c = c->peer; 
            }
            //Token_free(t); 
            return;
        }    
        case S_START : {

            // I may or may not start with data
            // S_START   -> S_TYPE, T_VAR, S_START_A;
            // and 
            // S_START_A -> T_LPAR etc...
            // implies a function , not data. 
            // Ive rolled up the S_START_A above to shrink tree. 

            
        };
    };
};

/** When a parser produces a empty symbol, it probably isnt useful
    this will clear these from the tree to make it smaller **/
int clearEmptySymbol( Token t ) {
    if( t->lpeer == NULL && t->peer == NULL ) t->parent->child = NULL;
    if( t->lpeer == NULL && t->peer != NULL ) t->parent->child = t->peer; 
    if( t->lpeer != NULL ) t->lpeer->peer = t->peer; 
    if( t->peer  != NULL ) t->peer->lpeer = t->lpeer; 
    Token_free( t ); 
};

/** When a parser produces a empty symbol, it probably isnt useful
    this will identify the cases where the symbol mathced a T_e **/
int isEmptySymbol( Token t ) {
    return ( ! isTerminal(t->type) && t->child == NULL );
}


void transform( Token t ) {

    if( t->peer  != NULL ) transform( t->peer  );

    if( isEmptySymbol(t) ) {
        clearEmptySymbol( t ); 
    } else {
        if( t->child != NULL ) transform( t->child ); 
        doTransform( t );
    }
}
#endif
