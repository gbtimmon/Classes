#ifndef _TRANSFORM_C_
#define _TRANSFORM_C_

#include "transform.h"
#include "assert.h"
#include <stdbool.h>

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

void findAndCollapse( Token t, token_t type ){
    Token c = Token_findChild( t, type ); 
    while( c != NULL ) {
        Token_collapse( c );     
        c = Token_findChild( t, type ); 
    }
};

void findAndRemove( Token t, token_t type ) {
    Token c = Token_findChild( t, type ); 
    while( c != NULL ) {
        Token_remove( c ); 
        Token_free( c ); 
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
 
        findAndRemove( data, T_SEMI ); 
        Token_appendChild( t, data ); 
 
        //then order the functions. 
        c = Token_findChild( t, S_XFUNC ); 
        findAndRemove( c, T_LPAR ); 
        findAndRemove( c, T_RPAR ); 

        while( c != NULL ){

            // for each function remove the parenthesis. 
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

void transformSFunc( Token t ) { 
    Token_collapse( t ); 
};

void transformSCode( Token t ) {
    Token c = Token_findChild( t, T_LPAR ); 
    Token d = Token_findChild( t, T_RPAR ); 

    Token_remove( c ); 
    Token_remove( d ); 

    Token_free( c ); 
    Token_free( d ); 
};

void transformSFuncDef( Token t ) {

    if( t->child->type == T_SEMI ) {
        Token_remove( t ); 
        Token_free( t ); 
        return; 
    }

    findAndRemove( t, T_LCURL ); 
    findAndRemove( t, T_RCURL ); 
    Token_collapse( t ); 
};

void transformSIdList( Token t ) {
     Token_remove( t->child ); 
     findAndCollapse( t, S_IDLIST ); 
};

void transformSData( Token t ) { 
     findAndCollapse( t, S_IDLIST ); 
     findAndCollapse( t, S_XDATA ); 
     t->type = S_XDATA; 
}


void doTransform( Token t ) {

    switch( t->type ) {
        // The type can collapse into a simpler element. 
        case S_TYPE        : transformSType( t );      break;
        case S_PARM_LIST_A : transformSParmListA( t ); break;
        case S_PARM_LIST   : transformSParmList( t );  break; 
        case S_START_A     : transformSStartA( t );    break;
        case S_START       : transformSStart( t );     break;
        case S_FUNC        : transformSFunc( t );      break; 
        case S_CODE        : transformSCode( t );      break; 
        case S_FUNC_DEF    : transformSFuncDef( t );   break; 
        case S_IDLIST      : transformSIdList( t );    break; 
        case S_DATA :        transformSData( t );      break;
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
};

void shapeTransform( Token t ) {

    if( t->peer  != NULL ) shapeTransform( t->peer  );

    if( isEmptySymbol(t) ) {
        clearEmptySymbol( t ); 
    } else {
        if( t->child != NULL ) shapeTransform( t->child ); 
        doTransform( t );
    }
};

symbol_t toType( const char * c ) {
    if( EQ( c, "int" ) ) return TYPE_INT; 
    if( EQ( c, "void" ) ) return TYPE_VOID; 
    if( EQ( c, "binary" ) ) return TYPE_BINARY; 
    if( EQ( c, "decimal" ) ) return TYPE_DECIMAL; 
    return TYPE_UNDEF; 
};

void loadVarList( SymbolTable s, Token t, bool referenced ) {
    // Data elements after transform have the form 
    // type var var var type var var etc ...
    // load a token in this form to a symbol table. 

    Token c = t->child; 
    symbol_t type = TYPE_UNDEF; 
     
    while( c != NULL ) { 
        if( c->type == T_XTYPE || c->type == T_TYPE ) 
           type = toType( c->value ); 
    	else if (c->type == T_VAR )
           SymbolTable_add( s, type, c->value, referenced ); 

        c = c->peer; 
    };
}
 
     
void buildSymbolTables( Token t ){

     // static analysis of the space we need. 
     Token data = Token_findChild( t, S_XDATA ); 
     int cnt1 = Token_countChild( data, T_VAR  ); 
     int cnt2 = Token_countChild( t, S_XFUNC ); 
     printf( "GLOBAL VARS -> %d vars, %d funcs\n", cnt1, cnt2 ); 

     t->scope = SymbolTable_new( cnt1 + cnt2 ); 
     loadVarList( t->scope, data, true ); 

     Token c = t->child; 
     while( c != NULL ) {
         if( c->type == S_XFUNC ) {
             Token type  = Token_findChild( c, T_XTYPE ); 
             Token name  = Token_findChild( c, T_VAR ); 
             Token parms = Token_findChild( c, S_PARM_LIST );
             Token fdata = Token_findChild( c, S_XDATA ); 

             cnt1 = Token_countChild( parms, T_VAR ); 
             cnt2 = Token_countChild( fdata, T_VAR ); 
             
             printf("FUNC ( %s %s ) -> %d parms, %d locals\n", type->value, name->value, cnt1, cnt2 ); 

             c->scope = SymbolTable_new( cnt1 + cnt2 ); 
             SymbolTable_add( t->scope, TYPE_FUNC, name->value, false ); 
             if( fdata != NULL ) 
                 loadVarList( c->scope, fdata, true ); 

             if( parms != NULL ) 
                 loadVarList( c->scope, parms, false); 


         }
         c = c->peer; 
     }
};

void transform( Token t ) {
    shapeTransform( t ); 
    buildSymbolTables( t ); 
} 

#endif
