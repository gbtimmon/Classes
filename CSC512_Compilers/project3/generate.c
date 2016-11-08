#ifndef _GENERATE_C_
#define _GENERATE_C_

#include <stdio.h>
#include "generate.h"

void writeMeta( FILE * f, Token t ) { 
   Token c = t->child; 
   while( c != NULL ) {
       fprintf( f, "GenMeta : %s\n", c->value ); 
       c = c->peer; 
   }
};       

void writeFunc( FILE * f, Token t ) {
   Token type = Token_findChild( t, T_XTYPE ); 
   if( type == NULL ) type = Token_findChild( t, T_TYPE ); 

   Token name = Token_findChild( t, T_VAR ); 
   Token parm = Token_findChild( t, S_PARM_LIST); 

   int local_cound = t->scope->ref; 

   fprintf( "int %s( %s )



}
void generate( FILE * f, Token t ) {

    int global_size = t->scope->ref;
    printf( "int GLOBAL[%d];\n", global_size ); 
   
    Token c = t->child; 
    while( c != NULL ) {
        if( c->type == S_XFUNC )
            writeFunc( f, c ); 
        c = c->peer; 
    };
};

#endif 
