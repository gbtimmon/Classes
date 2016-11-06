#ifndef _SYMBOL_TABLE_C_
#define _SYMBOL_TABLE_C_

typedef enum _symbol_t {  
    S_INT, 
    S_VOID, 
    S_BINARY, 
    S_DECIMAL
} symbol_t; 
    
typedef struct _symbol {
    symbol_t type;       
    char *   name; 
    char *   scope; 
    int      ref; 
} * Symbol; 

typedef struct _sTable {
    int size; 
    int cap; 
    struct * _symbol data; 
} * SymbolTable; 

SymbolTable SymbolTable_new ( int initSize );
void SymbolTable_free( SymbolTable s );
#endif
