#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

typedef enum _symbol_t {  
    TYPE_INT, 
    TYPE_VOID, 
    TYPE_BINARY, 
    TYPE_DECIMAL, 
    TYPE_UNDEF
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
void         SymbolTable_add ( SymbolTable, symbol_t, const char * ); 
void         SymbolTable_write( SymbolTable );
void         SymbolTable_free( SymbolTable );

#endif
