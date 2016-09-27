#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "buffer.h"
#include "token.h"

#define RESERVED_WORD_COUNT 13
 
typedef struct _scanner { 
    int lineNo;
    int errCnt; 
    int outFlag;
    char cur; 
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
