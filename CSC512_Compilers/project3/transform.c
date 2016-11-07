#ifndef _TRANSFORM_C_
#define _TRANSFORM_C_

#include "transform.h"
#include "assert.h"

int _break_; 
#define BREAK() {printf("BREAK %d\n", _break_++);fflush(NULL);}

#define EQ(x,y) strcmp( x, y ) == 0 

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

    int varCount = 0; 
    Token ele = c->child;
    while( ele != NULL ) { 
        if( ele->type == T_VAR ) varCount ++;
        ele = ele->peer; 
    }

    t->scope = SymbolTable_new( varCount ); 

    ele = c->child; 
    symbol_t sym = TYPE_UNDEF; 

    while( ele != NULL ) {
        if( ele->type == T_XTYPE ) {
             if( EQ( ele->value, "int" ) )     sym = TYPE_INT; 
             if( EQ( ele->value, "binary" ) )  sym = TYPE_BINARY; 
             if( EQ( ele->value, "void" ) )    sym = TYPE_VOID; 
             if( EQ( ele->value, "decimal" ) ) sym = TYPE_DECIMAL;
        } else if ( ele->type == T_VAR ) {
             SymbolTable_add( t->scope, sym, ele->value ); 
        } else if ( ele->type == T_SEMI ) {
             sym = TYPE_UNDEF; 
        } 
        ele = ele->peer; 
    }
};

void findAndCollapse( Token t, token_t type ){

    Token c = Token_findChild( t, type ); 
    while( c != NULL ) {
        Token_collapse( c );     
        c = Token_findChild( t, type ); 
    }
};

void transformSType( Token t ) {

    //The SType just wraps another type object.
    // so just delete the SType from the tree. 

    Token c   = t->child;              
    Token old = Token_replace( t, c ); 
    c->type   = T_XTYPE; 
    Token_free( old ); 
};

void transformSParmListA( Token t ) {
     Token c; 
  
     c = Token_remove( t->child ); //remove the comma.
     Token_free(c); 

     c = Token_remove( t->child ); //remove the ParmListB
     Token_join( t, c );           //append it to my list; 
};

void transformSParmList( Token t ) {
     findAndCollapse( t, S_PARM_LIST_A ); 
};


void transformSIdList( Token t ) {
     // I dont need the comma.  
     Token_remove( t->child ); 
     findAndCollapse( t, S_IDLIST ); 
};

void transformSStart( Token t ) {

    findAndCollapse( t, S_START_A ); 

    Token start = Token_findChild( t, S_START ); 
    Token func  = Token_findChild( t, S_XFUNC ); 

    if( start == NULL && func == NULL && t->parent == NULL ){
        Token func = Token_new( S_XFUNC, NULL );
        Token c = t->child; 
        Token n; 

        while( c != NULL ) {
            n = c->peer; 
            Token_remove( c ); 
            Token_appendChild( func, c );
            c = n; 
        }
   
        Token_appendChild( t, func ); 

    } else if( start == NULL && func == NULL ) {
       t->type = S_XFUNC;
    } else if ( start != NULL ) {
       Token_collapse( start );
    }

    //wrangle data in to a data table node. 
    if( t->parent == NULL ) {
        Token data = Token_new( S_XDATA, NULL ); 
        Token c    = Token_findChild( t, S_XFUNC )->lpeer; 
        Token next; 

        while( c != NULL ){
            next = c->lpeer; 
            Token_remove( c ); 
            Token_appendChild( data, c ); 
            c = next; 
        }
 
        Token_appendChild( t, data ); 
 
        buildSymbolTable( t ); 

        //then order the functions. 
        c = Token_findChild( t, S_XFUNC ); 
        while( c != NULL ){
            c = Token_findChild( c, S_CODE );
            if( c != NULL ){
                Token_remove( c );
                c->type = S_XFUNC;
                Token_prependChild( t, c ); 
            }
        }  
    }
}

void transformSStartA( Token t ) {
    findAndCollapse( t, S_IDLIST ); 
}; 

void transformSData( Token t ) {
     if( t->parent->type == S_DATA ) {
         //collapseUp(t); 
     } else if( t->parent->type == S_FUNC_DEF ) { 

     }
}; 

void doTransform( Token t ) {

    switch( t->type ) {
        // The type can collapse into a simpler element. 
        case S_TYPE        : transformSType( t );      break;
        case S_PARM_LIST_A : transformSParmListA( t ); break;
        case S_PARM_LIST   : transformSParmList( t );  break; 
        case S_IDLIST      : transformSIdList( t );    break;
        case S_START_A     : transformSStartA( t );    break;
        case S_START       : transformSStart( t );     break;
        //case S_DATA :        transformSData( t );      break;
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
