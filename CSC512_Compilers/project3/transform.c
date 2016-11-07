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

void buildSymbolTable( Token t ) {

    Token c = t->child; 
    while( c->type != S_XDATA ) c = c->peer; 

    Token ele = c->child;
    symbol_t  = -1; 

     
      

    printf("build symbol table\n"); 
};

void collapseUp( Token t ){ 
    // collapse upwards since I dont need depth. 
    Token c     = t->child; 
    Token first = t->child; 
    Token last  = t->child; 

    while( c != NULL ) {
        last = c; 
        c->parent = t->parent; 
        c = c->peer; 
    }

    first->lpeer   = t->lpeer; 
    if( t->lpeer != NULL ) 
        t->lpeer->peer = first; 
   
    last->peer = t->peer; 
    if( t->peer != NULL )
        t->peer->lpeer = last; 

    Token_free( t ); 
}

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
        case S_PARM_LIST_A :{ 
            //S_PARM_LIST_B is useless just delete it. 
            Token oldlist = t->child; 
            Token newlist = t->child->peer->child; //
            
            t->child = newlist; 
         
            Token c = t->child; 
            while( c != NULL ) {
                c->parent = t;
                c = c->peer; 
            }
   
            Token d = oldlist; 
            while( d != NULL ) {
                Token next = d->peer; 
                Token_free( d );   
                d = next;
            }

            collapseUp( t ); 
            
            return;
        }
        case S_IDLIST : {
     
            // I dont need the comma.  
            t->child = t->child->peer; 
            Token_free( t->child->lpeer );
            t->child->lpeer = NULL;
            
            collapseUp( t ); 
            return; 
        }
        case S_START_A : {
            collapseUp( t ); 
            return;
        }    
        case S_START : {

            // The lowest start is the start of the code. 
            // other wise starts will chain with data init. 
            Token c = t->child; 
            while( c != NULL ) {
               if( c->type == S_XCODE) break;
               c = c->peer; 
            }
            // If there no root below me, I am the start of code. 
            if( c == NULL ) {
                t->type = S_XCODE; 
            } else {
                if( t->parent != NULL ) 
                    collapseUp( t ); 
                else {
                    Token data = Token_new( S_XDATA, NULL ); 
                    c = t->child; 
                    while( c != NULL ) {
                        Token next = c->peer; 
                        if( c->type != S_XCODE ) {
                            c->parent = NULL;    
                            c->child  = NULL;
                            c->peer   = NULL;
                            c->lpeer  = NULL;
                            Token_prependChild( data, c ); 
                        } else {
                            c->lpeer = NULL;
                            c->peer  = NULL;
                            t->child = c;
                        }
                        c = next; 
                    }
                    Token_appendChild( t, data );  
 
                    buildSymbolTable( t ); 
                }
            }
            return; 
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
