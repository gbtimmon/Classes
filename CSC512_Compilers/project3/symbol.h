#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

typedef enum _symbol_t {  
    S_INT, 
    S_VOID, 
    S_BINARY, 
    S_DECIMAL, 
    S_UNDEF
} symbol_t; 
    
typedef struct _symbol {
    symbol_t type;       
    char *   name; 
    int      ref; 
} * Symbol; 

typedef struct _sTable {
    int size; 
    int cap; 
    struct _symbol ** data; 
} * SymbolTable; 

SymbolTable  SymbolTable_new ( int );
void         SymbolTable_free( SymbolTable );
Symbol       Symbol_new( symbol_t , const char * );
void         Symbol_free( Symbol ); 
endif
