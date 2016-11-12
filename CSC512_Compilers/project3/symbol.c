#ifndef _SYMBOL_TABLE_C_
#define _SYMBOL_TABLE_C_

#include "symbol.h" 
#include "stdio.h"

#define EQ(x,y) ( strcmp( x, y ) == 0 ) 
SymbolTable SymbolTable_new ( int initSize ) { 

    SymbolTable new = malloc( sizeof( struct _sTable ) ); 
    new->data  = malloc( sizeof( struct _symbol * ) * initSize ); 
    new->size  = 0;         // how many total names
    new->ref   = 0;         // how many referenced vars. 
    new->temp  = 0;         // how many temps I can allocate. 
    new->tempa = 0;         // how many temps I have allocated. 
    new->cap   = initSize;  // how many names i might store
}; 

void SymbolTable_free( SymbolTable s ){
    for( int i = 0; i < s->cap; i++ ) { 
        Symbol_free( s->data[i] ); 
    }
    free( s->data ); 
    free( s ); 
};

int  SymbolTable_addTemp( SymbolTable s, int count ){
     s->temp += count; 
};

int  SymbolTable_getTemp( SymbolTable s ) {
     s->tempa = s->tempa + 1; 
     return s->ref + s->tempa - 1; 
}

int  SymbolTable_getAllocSize( SymbolTable s ) { 
     return s->ref + s->temp; 
};

int  SymbolTable_add( SymbolTable s,  symbol_t type, int size, const char * name, bool referenced ){
  
    if( SymbolTable_find( s, name ) != NULL ) 
        return -1; 

    Symbol new = malloc( sizeof( struct _symbol ) ); 
    new->type = type; 
    new->size = size; 
    new->name = malloc( sizeof(char) * (strlen( name ) + 2) );
    strcpy( new->name, name ); 

    if( referenced ) {
        new->ref = s->ref; 
        s->ref += size;  
    } else 
        new->ref = -1; 

    s->data[ s->size ] = new; 
    s->size = s->size + 1; 
    return 0; 
};

Symbol SymbolTable_find( SymbolTable s, const char * name ) {
    
    if( s == NULL ) printf("ASASDFASFASF"); 

    for( int i = 0; i < s->size; i++ ) {
        Symbol t = s->data[i]; 
        if( EQ(t->name, name ) ) return t;
    }
    return NULL;
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
        if( t->type == TYPE_FUNC )    type = "func"; 
        if( t->type == TYPE_UNDEF )   type = "undef";

        printf("   Symbol [ %s %s %d ]\n", type, t->name, t->ref ); 
    }
}

void Symbol_free( Symbol s ) {
    free( s->name );  
    free( s ); 
}; 
    
    



#endif
