#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "buffer.h"
#include "token.h"

#define RESERVED_WORD_COUNT 13
/*
 Code to create a new scanner object. 
  This scanner will produce the token define in project one but has a recode function 
  builtin that takes automatically generated code from the generateTable.py script that 
  will convert generic tokens to specialized tokens. 
*/
typedef struct _scanner { 
    int lineNo;
    int errCnt; 
    int outFlag;
    char cur; 
    const char * fileName; 
    Buffer buffer; 
    token_t token;
    FILE * in;
    FILE * out;
} * Scanner;

Scanner Scanner_new( const char * inFile );
void    Scanner_free      ( Scanner s );
void    Scanner_setOutput ( Scanner s, int output );
Token   Scanner_nextToken ( Scanner s);
void    Scanner_nextChar  ( Scanner s );

const char* reserved_word[ RESERVED_WORD_COUNT ];

#endif
