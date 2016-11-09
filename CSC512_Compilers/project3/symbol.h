#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdbool.h>

typedef enum _symbol_t {  
    TYPE_INT, 
    TYPE_VOID, 
    TYPE_BINARY, 
    TYPE_DECIMAL, 
    TYPE_FUNC, 
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
    int ref;
    int temp; 
    int tempa; 
    struct _symbol ** data; 
} * SymbolTable; 

SymbolTable  SymbolTable_new ( int );
int          SymbolTable_add ( SymbolTable, symbol_t, const char *, bool ); 
int          SymbolTable_addTemp( SymbolTable, int );
int          SymbolTable_getAllocSize( SymbolTable );
int          SymbolTable_getTemp( SymbolTable );
void         SymbolTable_write( SymbolTable );
Symbol       SymbolTable_find( SymbolTable, const char * );
void         SymbolTable_free( SymbolTable );

#endif
