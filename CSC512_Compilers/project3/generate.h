#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>
#include "token.h"

void generate( FILE * f, Token meta, Token t );


char *      getReference     ( SymbolTable, SymbolTable, Token, int ); 
int         putOperChain     ( SymbolTable, SymbolTable, Token, int );
char *      putStatemenat    ( SymbolTable, SymbolTable, Token, int, int, int );
void        putWhileStatement( SymbolTable, SymbolTable, Token, int  ); 
void        putIfStatement   ( SymbolTable, SymbolTable, Token, int, int, int ); 
void        writeStatements  ( SymbolTable, SymbolTable, Token, int, int, int ); 
Token       getOffset        ( Token );
SymbolTable getLocalScope    ( Token );
SymbolTable getGlobalScope   ( Token );
void        putParms         ( Token );
void        loadParms        ( SymbolTable , Token, int );
void        writeFunc        ( FILE *, Token );
#endif 
