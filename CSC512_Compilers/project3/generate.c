#ifndef _GENERATE_C_
#define _GENERATE_C_

#include <stdio.h>
#include "generate.h"
#include "symbol.h"

void writeMeta( FILE * f, Token t ) { 
   Token c = t->child; 
   while( c != NULL ) {
       fprintf( f, "GenMeta : %s\n", c->value ); 
       c = c->peer; 
   }
};       

const char * tab_str ="    ";

void tab( int depth ) {
   for( int i = 0; i < depth; i++ ) printf(tab_str);       
};
   
void putLiteral( Token c ) { 
     printf( "%s ", c->value); 
}

void putReference( Token c ) { 

    // get the function head --> may be arbitrarily deep if im in a block in a block in a ....etc. 
    Token func = c->parent;  

    while( func->type != S_XFUNC ){
        func = func->parent; 
    }

    Symbol local = SymbolTable_find( func->scope, c->value ); 
      

    Token root = func->parent;

    while( root->type != S_START ) {
        root = root->parent ; 
    }

    Symbol global = SymbolTable_find( root->scope, c->value ); 


    if( local != NULL ){
       int ref = local->ref; 
       if( ref == -1 ) {
           printf("%s ", c->value); 
       } else {
           printf("local[%d] ", ref ); 
       }

    } else if ( global != NULL ) {
       int ref = global->ref; 
       if( ref == -1 ) {
           printf("%s ", c->value); 
       } else {
           printf("global[%d] ", ref ); 
       }
    } else {
       fprintf(stderr, "Undefined reference to variable %s.\n", c->value ); 
    }
}

void writeStatement( Token t, int depth ) {
    Token c = t->child; 

    tab( depth ); 
    while( c != NULL ) {
        switch ( c->type ) {
            case T_VAR : {
                putReference( c ); 
            break; }
            case T_LPAR:
            case T_RPAR:
            case T_EQ : {
                putLiteral( c ); 
            break; }
        }
        c = c->peer; 
    }
    printf(";\n");
};


void writeStatements( Token t, int depth ) { 
    Token c = t->child; 
    while( c != NULL ) {
        writeStatement( c, depth ); 
        c=c->peer; 
    }
};
    

void writeFunc( FILE * f, Token t ) {

   Token type = Token_findChild( t, T_XTYPE ); 
   Token name = Token_findChild( t, T_VAR ); 
   Token parm = Token_findChild( t, S_PARM_LIST ); 
   Token data = Token_findChild( t, S_XDATA ); 
   Token stat = Token_findChild( t, S_XCODE ); 

   int local_count = t->scope->ref; 

   printf( "int %s ( parms )", name->value ); 

   if( stat == NULL )
      printf( ";\n"); 
   else {
      printf( "{\n"); 
      if( local_count > 0 ) printf("    int local[%d];\n", local_count ); 
      writeStatements( stat, 1 ); 
      printf("};\n"); 
   }
};

void generate( FILE * f, Token t ) {

    int global_size = t->scope->ref;
    printf( "int global[%d];\n", global_size ); 
   
    Token c = t->child; 
    while( c != NULL ) {
        if( c->type == S_XFUNC )
            writeFunc( f, c ); 
        c = c->peer; 
    };
};

#endif 
