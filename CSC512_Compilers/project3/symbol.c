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

Symbol Symbol_new( symbol_t type, const char * name ) { 
  
    Symbol s = malloc( sizeof( struct _symbol ) ); 
    s->type = type; 
    s->name = malloc( sizeof(char) * (strlen( name ) + 2) );
    strcpy( s->name, name ); 

    return s; 
};

void Symbol_free( Symbol s ) {
    free( s->name );  
    free( s ); 
}; 
    
    



#endif
