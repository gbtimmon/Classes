#ifndef _TRANSFORM_C_
#define _TRANSFORM_C_

#include "transform.h"
#include "assert.h"
#include <stdbool.h>

int _break_; 
#define BREAK() {printf("BREAK %d\n", _break_++);fflush(NULL);}
#define SIZE_OF_ARRAY(_array) (sizeof(_array) / sizeof(_array[0]))
#define EQ(x,y) strcmp( x, y ) == 0 

/** Recode the IR Tree formed by the parser. 
    Do a depth / last item first scan and recode as we go. 
    This way subtrees are recoded before we recode a node. 
     **/

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
            Token_prependChild( func, c );
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

        //now for each func, wrangle the code. 
        Token func = t->child; 
        while( func != NULL ){
            if( func->type == S_XFUNC ){
                Token statement = func->child; 
                Token code      = Token_new( S_XCODE, NULL);
                while( statement != NULL ) {
                    Token next = statement->peer; 
                    if( statement->type == S_STATEMENT ) {
                        Token_remove( statement ); 
                        Token_prependChild( code, statement ); 
                    }
                    statement = next; 
                }
                if ( code->child != NULL ) {
                    Token_prependChild( func, code ); 
                };
            }
            func = func->peer; 
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
    findAndRemove( t, T_LPAR ); 
    findAndRemove( t, T_RPAR ); 
};

void transformSFuncDef( Token t ) {
    findAndRemove( t, T_LCURL ); 
    findAndRemove( t, T_RCURL ); 
    Token_collapse( t ); 
};

void transformSIdList( Token t ) {
     Token_remove( t->child ); 
     findAndCollapse( t, S_IDLIST ); 
};

void transformSData( Token t ) { 
     findAndCollapse( t, S_DATA_A ); 
     findAndCollapse( t, S_IDLIST ); 
     findAndCollapse( t, S_XDATA ); 
     t->type = S_XDATA; 
};
 
     
void transformSStatement( Token t ) {

    Token c = t->child;
    while( c != NULL ) {
        Token next = c->peer; 
        if( 
               c->type == S_STATEMENT_A
            || c->type == S_STATEMENT_B
            || c->type == S_STATEMENT_C
            || c->type == S_STATEMENT_D
            || c->type == S_STATEMENT_P
            || c->type == S_STATEMENT_R 
        ){
            Token_collapse( c ); 
        }
        c = next; 
    };

    Token d = Token_findChild( t, S_STATEMENT ); 
    while( d != NULL ) {
        Token_remove( d );
        Token_prependChild( t->parent, d ); 
        d = Token_findChild( t, S_STATEMENT ); 
    };
};

void transformTSemi( Token t ) {
    Token_remove( t ); 
    Token_free( t ); 
};

void transformSBlock( Token t ) {
    findAndRemove( t, T_LCURL );
    findAndRemove( t, T_RCURL ); 
};

void splitOper( Token t ) {
    Token cur = t; 
    while( cur != NULL ){
        int len = Token_countChild( cur, -1 );
        if( len > 3 ) {
            Token oper  = Token_new( S_XOPER, NULL ); 
            Token child = cur->child; 
            while( child->peer != NULL ) child = child->peer; 
    
            child = child->lpeer->lpeer; 
            while( child != NULL ){
                Token next = child->lpeer; 
                Token_remove( child );    
                Token_appendChild( oper, child ); 
                child = next;
            } 
            Token_appendChild( cur, oper);
            cur = oper; 
        } else {
            return;
        }
    }
};

void transformSExp( Token t ){
    findAndCollapse( t, S_EXP_A ); 

    if( t->child->peer == NULL ) {
        Token_replace( t, t->child ); 
        Token_free( t ); 
    };

    t->type = S_XOPER;
    splitOper( t ); 
};

void transformSTerm( Token t ){
    findAndCollapse( t, S_TERM_A ); 

    if( t->child->peer == NULL ) {
        Token_replace( t, t->child ); 
        Token_free( t ); 
    };
   
    t->type = S_XOPER; 
    splitOper( t ); 
};

void transformSFact( Token t ) {
    findAndCollapse( t, S_FACT_A ); 
    findAndCollapse( t, S_EXP_LIST );
    if( t->child->peer == NULL ){
       Token_replace( t, t->child ); 
       Token_free( t );
    } else if( t->child->type == T_LPAR ){
       Token c = Token_findChild( t, S_XOPER ); 
       Token_replace( t, c ); 
       Token_free( t ); 
    } else {
       t->type = S_XOPER;
    };
};
    
void transformSCondExp( Token t ){
    findAndCollapse( t, S_COND_EXP_A ); 

    int cc = Token_countChild( t, -1); 
    if( cc == 1 ){
        findAndCollapse( t, S_COND ); 
    } 
   
    if( cc == 3 ) {
        int lhs = Token_countChild(t->child, -1 ); 
        if( lhs > 1 ) 
            t->child->type = S_XOPER; 
        else 
            Token_collapse( t->child ); 
        
        int rhs = Token_countChild( t->child->peer->peer, -1 ); 
        if( lhs > 1 ) 
            t->child->peer->peer->type = S_XOPER; 
        else 
            Token_collapse( t->child->peer->peer ); 
    }
};
    
void transformSCond( Token t ){
};

void transformSAdd( Token t ){
    Token_replace( t, t->child ); 
    Token_free( t ); 
}
   
void transformSExpList( Token t ){
    findAndCollapse( t, S_EXP_LIST_A ); 
    Token_collapse( t ); 
};

void transformSExpListA( Token t ){
    findAndCollapse( t, S_EXP_LIST_A ); 
    findAndCollapse( t, S_EXP_LIST_B ); 
};

void transformSBrackExp( Token t ){
    findAndRemove( t, T_RBRACK ); 
    findAndRemove( t, T_LBRACK ); 
}
void doTransform( Token t ) {

    switch( t->type ) {
        // The type can collapse into a simpler element. 
        case S_BLOCK       : transformSBlock( t );     break;
        case S_COND        : transformSCond( t );      break; 
        case T_SEMI        : transformTSemi( t );      break; 
        case S_TYPE        : transformSType( t );      break;
        case S_PARM_LIST_A : transformSParmListA( t ); break;
        case S_PARM_LIST   : transformSParmList( t );  break; 
        case S_START_A     : transformSStartA( t );    break;
        case S_BRACK_EXP   : transformSBrackExp( t );  break;
        case S_ADD         : transformSAdd( t );       break;
        case S_COND_EXP    : transformSCondExp( t );   break; 
        case S_START       : transformSStart( t );     break;
        case S_FUNC        : transformSFunc( t );      break; 
        case S_EXP_LIST    : transformSExpList( t );   break;
        case S_EXP_LIST_A  : transformSExpListA( t );  break;
        case S_CODE        : transformSCode( t );      break; 
        case S_EXP         : transformSExp( t );       break; 
        case S_TERM        : transformSTerm( t );      break;
        case S_FACT        : transformSFact( t );      break;
        case S_FUNC_DEF    : transformSFuncDef( t );   break; 
        case S_IDLIST      : transformSIdList( t );    break; 
        case S_DATA        : transformSData( t );      break;
        case S_STATEMENT   : transformSStatement( t ); break;
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

int isVarUsed( Token root, Token var ) {
    if( root->type == S_XFUNC ) {
        return isVarUsedInFunc( root, var ); 
    }

    Token c = root->child;  
    while( c != NULL ) {
        if( c->type == S_XFUNC ){
            int retval = isVarUsedInFunc( c, var ); 
            if( retval ) return retval; 
        }
        c = c->peer; 
    }
    return 0;
}

int isVarUsedInFunc( Token func, Token var ) {

    Token code = Token_findChild( func, S_XCODE );
    return _dfs( code, var ); 
}

int _dfs( Token node, Token target ){
    if( node == NULL )
        return false; 

    if( node->value != NULL && EQ(node->value, target->value ) )
        return true; 

    if( node->child == NULL ) 
        return false; 

    Token child = node->child; 
    while( child != NULL ){
        if( _dfs( child, target) ) 
            return true; 
        child = child->peer; 
    }
    return false; 
}

int getDeclarationSize( Token c ) {
    if(c->peer == NULL )              return 1; 
    if(c->peer->type != S_BRACK_EXP ) return 1;
    if(c->peer->child == NULL ) {
       fprintf(stderr, "Compile Error : array declaration with no size\n"); 
       exit(1); 
    }
    if(c->peer->child->type != T_NUMBER ) {
       fprintf(stderr, "Compile Error : illegal array declaration, only constant size arrays allowed.\n"); 
       exit(1); 
    }

    int value = atoi( c->peer->child->value ); 

    if( value == 0 ) {
       fprintf(stderr, "Compile Error : illegal array declaration, only constant size arrays allowed.\n"); 
       exit(1); 
    }

    return value; 
}
void loadVarList( Token container, Token data ) {
    // Data elements after transform have the form 
    // type var var var type var var etc ...
    // load a token in this form to a symbol table. 

    Token c = data->child; 
    SymbolTable s = container->scope; 
    symbol_t type = TYPE_UNDEF; 
     
    while( c != NULL ) { 
        if( c->type == T_XTYPE || c->type == T_TYPE ) 
           type = toType( c->value ); 
    	else if (c->type == T_VAR){
            if( isVarUsed( container, c ) ){
                SymbolTable_add( container->scope, type, getDeclarationSize( c ), c->value, true  ); 
            }
            else
                printf( "INFO : removing unused var %s.\n", c->value ); 
        }
        c = c->peer; 
    };
}

int countOper( Token t ){
    int cnt = 0; 
    if( t->type == S_XOPER ) cnt++; 
  
    Token c = t->child; 
    while( c != NULL ) {
       cnt += countOper( c ); 
       c = c->peer; 
    }
    return cnt;
}

    
 
void buildSymbolTables( Token t ){

     // static analysis of the space we need. 
     Token data = Token_findChild( t, S_XDATA ); 
     int cnt1 = Token_countChild( data, T_VAR  ); 
     int cnt2 = Token_countChild( t, S_XFUNC ); 

     t->scope = SymbolTable_new( cnt1 + cnt2 ); 
     loadVarList( t, data ); 

     Token c = t->child; 
     while( c != NULL ) {
         if( c->type == S_XFUNC ) {
             Token type  = Token_findChild( c, T_XTYPE ); 
             Token name  = Token_findChild( c, T_VAR ); 
             Token parms = Token_findChild( c, S_PARM_LIST );
             Token fdata = Token_findChild( c, S_XDATA ); 

             cnt1 = Token_countChild( parms, T_VAR ); 
             cnt2 = Token_countChild( fdata, T_VAR ); 
             
             c->scope = SymbolTable_new( cnt1 + cnt2 ); 

             // Count the temp vars. 
             SymbolTable_addTemp(c->scope, countOper( c ) ); 

             SymbolTable_add( t->scope, TYPE_FUNC, 1, name->value, false ); 
             if( fdata != NULL ) 
                 loadVarList( c, fdata ); 

             if( parms != NULL ) 
                 loadVarList( c, parms ); 


         }
         c = c->peer; 
     }
};

void transform( Token t ) {
    shapeTransform( t ); 
    buildSymbolTables( t ); 
};

#endif
