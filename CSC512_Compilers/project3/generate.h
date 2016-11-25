#ifndef _GENERATE_H_
#define _GENERATE_H_

#include <stdio.h>
#include "token.h"
/**
  Takes a fully transformed token tree and outputs the code. 
  
  Only real public function is generate, but there is a lot of 
  cyclic function calls so I just moved all function declarations to 
  the header to make my life easy. 

  generate creates the global vars and the meta statements and starts each function. 

  Each function fills its blocks in with put statements. 

  each statement is written with put statment, any time a refernence is need get
  reference is calls to consult the scope objects and return how to call the object. 
  If there is a offset that is created on getReference also.  Any compund operation will be 
  marked as a S_XOPER and putOperChain will create all of the compund statements before returning 
  reference to where the result of that compound operation is stored. 

  There are special putStatment functions for if and while statements, that create the lable
  based flow control and make a inner call to write statements to create the inner block. 

  Part of the function parameters for writestatments, putWhileStatement and putIfStatment is
  a label reference for exiting a loop or continuing to the next iteration of the loop. putWhileStatement
  will pass in the labels for its llop start label and loop exit label so any break or continue will 
  know where to goto. The initial writeStatements sets the label to -1 and any break or continue with a -1
  label will result in an error continue or break without containing loop. 
**/
  
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
