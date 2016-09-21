#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "./buffer.c"
#include "./token.c"


typedef struct _scanner { 
    int lineNo;
    int errCnt; 
    int outFlag;
    int curC; 
    Buffer b; 
    FILE * in;
    FILE * out;
} * Scanner;

Scanner Scanner_new( const char * inFile, const char * outFile ) {
void Scanner_free( Scanner s ) {
void Scanner_setOutput( Scanner s, int output ) {
void Scanner_next( Scanner s, Token t ) {

const char* reserved_word[ RESERVED_WORD_COUNT ] = {
    "main", //I cheated a little. 
    "int",
    "void",
    "if",
    "while",
    "return",
    "read",
    "write",
    "print",
    "continue",
    "break",
    "binary",
    "decimal"
};


#endif
