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
const char * tab_str_cheat ="   "; // OMG What a hack. See what happens when I code for 10 hours straight?!.

void tab( int depth ) {
   for( int i = 0; i < depth; i++ ) printf(tab_str);       
};

void tabCheat( int depth ){
   for( int i = 0; i < depth-1; i++ ) printf(tab_str);       
   if( depth > 0 ) printf( tab_str_cheat ); 
}
   
char * extendBuf( char * old, int newSize ){
    char * new = malloc( sizeof( char ) * newSize); 
    strcpy( new, old ); 
    free( old ); 
    return new; 
}

char * getReference( SymbolTable global, SymbolTable local,  Token c ) { 

    Symbol locS = SymbolTable_find( local, c->value ); 
    Symbol gloS = SymbolTable_find( global, c->value ); 


    char * ret = malloc( sizeof( char ) * 20 ); 

    if( locS != NULL ){
       int ref = locS->ref; 
       if( ref == -1 ) {
           sprintf(ret, "%s", c->value); 
       } else {
           sprintf(ret, "local[%d]", ref ); 
       }

    } else if ( gloS != NULL ) {
       int ref = gloS->ref; 
       if( ref == -1 ) {
           sprintf( ret, "%s", c->value); 
       } else {
           sprintf( ret, "global[%d]", ref ); 
       }
    } else {
       fprintf(stderr, "Undefined reference to variable %s.\n", c->value ); 
       exit(1); 
    }
    return ret;
}
int putOperChain(SymbolTable global, SymbolTable local, Token t, int depth ) {

    int ref = SymbolTable_getTemp( local ); 

    int buf  = 100; 
    int used = 0;  
    char * out = malloc( sizeof( char ) * buf );   

    sprintf( out, "local[%d] =", ref ); 
    used = strlen( out ); 
    
    Token c = t->child; 
    while ( c != NULL ) {
        if( c->type == S_XOPER ) {
           
            // Make sure I have space. 
            if( used + 10 > buf ){
                buf = buf * 2; 
                out = extendBuf( out, buf ); 
            }
            int newref = putOperChain( global, local, c, depth); 
            sprintf( out, "%s local[%d]", out, newref ); 
            used = strlen( out ); 
        } else if ( c->type == T_VAR ) {
            char * refName = getReference( global, local, c ); 
            
            if( used + strlen(refName) + 2 > buf ){
                buf = buf*2; 
                out = extendBuf( out, buf ); 
            }

            sprintf( out, "%s %s", out, refName ); 
            free( refName );
        } else {

            int newSize = strlen(c->value ) + 3; 
            if( used + newSize > buf ) {
                buf = buf * 2; 
                out = extendBuf( out, buf ); 
            };
            sprintf( out, "%s %s", out, c->value); 
            used = strlen(out);           
        }

        c = c->peer; 
    }   
    tab(depth);
    printf( "%s;\n", out ); 
    free(out); 
    return ref; 
};

void putLiteral( Token c ) { 
     printf( "%s ", c->value); 
}

void putStatement( SymbolTable global, SymbolTable local, Token t, int depth ){
    Token c = t->child; 

    int buf  = 100; 
    int used = 0; 
    char * out = malloc( sizeof( char ) * buf ); 
    sprintf( out, "" ); 

    while( c != NULL ){
        if( c->type == T_VAR ){ 
            char * name = getReference( global, local, c ); 
           
            if( strlen(name) + used + 5 > buf ){
                buf = buf * 2; 
                out = extendBuf( out, buf ); 
            }
            sprintf(out, "%s %s", out, name ); 
            free( name );
        } else if( isTerminal(c->type) ) {

            if( strlen(c->value) + used + 3 > buf ){
                buf = buf * 2; 
                out = extendBuf( out, buf ); 
            }

            sprintf( out, "%s %s", out, c->value ); 
        } else if( c->type == S_XOPER ) { 
       
            int ref = putOperChain( global, local, c, depth ); 
            
            if( used + 20 > buf ){ 
                buf = buf * 2; 
                out = extendBuf( out, buf); 
            }

            sprintf( out, "%s local[%d]", out, ref); 
        }
            
        used = strlen(out); 
        c= c->peer; 
    }
    tabCheat(depth);
    printf( "%s;\n", out );
}
void writeStatements( SymbolTable global, SymbolTable local, Token t, int depth ) { 
    Token c = t->child; 
    while( c != NULL ) {
        putStatement( global, local, c, depth ); 
        c=c->peer; 
    }
};
    

SymbolTable getLocalScope( Token t ){
    Token c = t; 
    while( c-> type != S_XFUNC ) c=c->parent; 
    return c->scope; 
}

SymbolTable getGlobalScope( Token t ){
    Token c = t; 
    while( c-> type != S_START ) c=c->parent; 
    return c->scope; 
}

void writeFunc( FILE * f, Token t ) {

   SymbolTable local  = getLocalScope( t ); 
   SymbolTable global = getGlobalScope( t ); 

   Token type = Token_findChild( t, T_XTYPE ); 
   Token name = Token_findChild( t, T_VAR ); 
   Token parm = Token_findChild( t, S_PARM_LIST ); 
   Token data = Token_findChild( t, S_XDATA ); 
   Token stat = Token_findChild( t, S_XCODE ); 

   int local_count = SymbolTable_getAllocSize(local); 

   printf( "int %s ( parms )", name->value ); 

   if( stat == NULL )
      printf( ";\n\n"); 
   else {
      printf( "{\n\n"); 
      if( local_count > 0 ) printf("    int local[%d];\n\n", local_count ); 
      writeStatements( global, local, stat, 1 ); 
      printf("};\n"); 
   }
};

void generate( FILE * f, Token t ) {

    int global_size = SymbolTable_getAllocSize( t->scope ); 
    printf( "int global[%d];\n\n", global_size ); 
   
    Token c = t->child; 
    while( c != NULL ) {
        if( c->type == S_XFUNC )
            writeFunc( f, c ); 
        c = c->peer; 
    };
};

#endif 
