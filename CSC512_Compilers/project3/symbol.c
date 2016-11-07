#ifndef _SYMBOL_TABLE_C_
#define _SYMBOL_TABLE_C_

#include "symbol.h" 

SymbolTable SymbolTable_new ( int initSize ) { 

    SymbolTable new = malloc( sizeof( struct _sTable ) ); 
    new->data = malloc( sizeof( struct _symbol * ) * initSize ); 
    new->size = 0; 
    new->cap  = initSize; 
}; 

void SymbolTable_free( SymbolTable s ){
    for( int i = 0; i < s->cap; i++ ) { 
        Symbol_free( s->data[i] ); 
    }
    free( s->data ); 
    free( s ); 
};

void SymbolTable_add( SymbolTable s,  symbol_t type, const char * name ){
  
    Symbol new = malloc( sizeof( struct _symbol ) ); 
    new->type = type; 
    new->name = malloc( sizeof(char) * (strlen( name ) + 2) );
    strcpy( new->name, name ); 

    s->data[ s->size ] = new; 
    s->size = s->size + 1; 
};

void SymbolTable_write( SymbolTable s ) {
    printf( "--SYMBOLS--\n" );
    for( int i = 0; i < s->size; i++ ) {
        Symbol t = s->data[i]; 
        char * type; 
        if( t->type == TYPE_INT )     type = "int";
        if( t->type == TYPE_VOID )    type = "void";
        if( t->type == TYPE_DECIMAL ) type = "decimal";
        if( t->type == TYPE_BINARY )  type = "binary";
        if( t->type == TYPE_UNDEF )   type = "undef";

        printf("   Symbol [ %s %s ]\n", type, t->name ); 
    }
}

void Symbol_free( Symbol s ) {
    free( s->name );  
    free( s ); 
}; 
    
    



#endif
