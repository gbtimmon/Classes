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
   Token name = Token_findChild( t, T_VAR ); 
   Token parm = Token_findChild( t, S_PARM_LIST ); 
   Token data = Token_findChild( t, S_XDATA ); 
   Token stat = Token_findChild( t, S_STATEMENT ); 

   int local_count = t->scope->ref; 

   printf( "int %s ( parms )", name->value ); 

   if( stat == NULL )
      printf( ";\n"); 
   else {
      printf( "{\n"); 
      if( local_count > 0 )
          printf("    int local[%d];\n", local_count ); 
      printf("};\n"); 
   }
}

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
