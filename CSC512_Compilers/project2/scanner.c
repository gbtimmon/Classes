#ifndef _SCANNER_C_
#define _SCANNER_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "./buffer.c"
#include "./token.c"

int LINE_COUNT  = 1; 
int ERROR_COUNT = 0;

int charIn( const char c, char * string );
int isReserved( Buffer b );
char * outFile( const char * inFile );
char consumeIdent( Buffer b, char first );
char consumeNumber( Buffer b, char first );
char consumeMeta( Buffer b, char first );
char consumeString( Buffer b, char first );
char consumeComment( Buffer b );
char consumeMulitlineComment( Buffer b );
char consumeOp( Buffer b, char first );

// I want my "Parser" to "have" a scanner, so lets make scanner OOP.
//
// We need to:
//    1.) take our redirects and make them non global. 
//    2.) store state so I can demand a token as they are needed by the object owner. 
//    3.) Implement hasNext() 
//    4.) Implement getNext()
//    5.) Implement constructor destructor. 
//    6.) Implement objects for terminals (structs with label)


typedef struct _scanner { 
    int lineNo;
    int errCnt; 
    int outFlag;
    Token token; 
    int cur; 
    Buffer buffer; 
    FILE * in;
    FILE * out;
} * Scanner;

Scanner Scanner_new( const char * inFile, const char * outFile ) {
    Scanner n = malloc( sizeof (struct _scanner) );

    n->lineNo  = 0; 
    n->errCnt  = 0;
    n->outFlag = 0;
    n->in      = ( inFile == NULL) ? stdin : fopen( inFile, "r");
    n->out     = ( outFile == NULL)? stdout: fopen(outFile, "w");
    n->buffer  = Buffer_new(); 
    n->token   = Token_new();
    n->cur     = getc( n->in );
}

void Scanner_free( Scanner s ) {
    Buffer_free( s->buffer );
    free(s); 
}

void Scanner_setOutput( Scanner s, int output ) {
    s->outFlag = output;
};

// This will load the Scanner->token data with the next good token. 
// It will overwrite the last token. The return will return the same 
// memory location everytime, so the return is just for synatic sugar. 
Token Scanner_next( Scanner s ) {
   
    if( s->cur == '\n') 
        s->lineNo++;

    // One of these braches will build a new token. 
    // if I get to the final else, I will skip a char and try again. 
    // otherwise ive built a new good token.
    if( s->cur == EOF )
        printf("EOF_Token\n");
    
    else if( isalpha( s->cur ) || s->cur == '_' ) 
         s->cur = consumeIdent( s->buffer, s->cur );

    else if ( isdigit( s->cur ) ) 
         s->cur = consumeNumber( s->buffer, s->cur ); 

    else if ( s->cur == '#' )
         s->cur = consumeMeta( s->buffer, s->cur );

    else if ( s->cur == '"' )
         s->cur = consumeString( s->buffer, s->cur);

    else if( charIn(s->cur, "(){}[],;+-*/<>=|&!") ) 
         s->cur = consumeOp( s->buffer, s->cur );

    else { 
         s->cur = getc( s->in );
         Scanner_next( s ); 
    } 
    
    return s->token;
}


typedef FILE* File; 
#define RESERVED_WORD_COUNT 13
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

int charIn( const char c, char * string ) {
    char * i = string; 
    while( *i != '\0' ){
        if( *i == c )
           return 1;
        i++;
    }
    return 0; 
};

int isReserved( Buffer b ) {
    for ( int i = 0; i < RESERVED_WORD_COUNT; i++ ) {
        if( strcmp( reserved_word[ i ] , b->stack ) == 0 )
            return 1; 
    }
    return 0;
};

char * outFile( const char * inFile ) {

     char * newString = ( char * ) malloc( sizeof( char ) * ( strlen(inFile) + 4 ) );
    
     int strIndex = strlen(inFile) + 4; //I add four chars and its index from 0.

     for ( int i = strlen(inFile); i >= 0; i--){
       
         newString[ strIndex-- ] = inFile[i];

         if( inFile[i] == '.' ) {
             newString[ strIndex-- ] = 'n';
             newString[ strIndex-- ] = 'e';
             newString[ strIndex-- ] = 'g';
             newString[ strIndex-- ] = '_';
         }
     }

    return newString;
}

char consumeIdent( Buffer b, char first ) {

    char cur = first; 
    Buffer_reset( b );

    while( true ) {
        if( isalpha(cur) || isdigit(cur) || cur == '_' ){
            Buffer_write( b, cur );
        } else {
            if( isReserved ( b ) ) {
                Buffer_emit( "KEYWD", "", b, false );
            } else {
                Buffer_emit( "IDENT", "cs512", b, false );
            }
            return cur;
        }
        cur = getc( stdin );
    }
}
     
char consumeNumber( Buffer b, char first ){

    char cur = first; 
    Buffer_reset( b ); 

    while( true ) {
        if( isdigit( cur ) ) {
            Buffer_write( b, cur);
        } else {
            Buffer_emit( "NUMBR", "", b, false);         
            return cur; 
        }
        cur = getc( stdin );
    }
};

char consumeMeta( Buffer b, char first ) {

    char cur = first; 
    Buffer_reset( b ); 

    while( true ) {
        if( cur == '\n'){
            LINE_COUNT++;
            Buffer_emit( "MSTMT", "", b, true );         
            return getc( stdin ); 
        } else {
            Buffer_write( b, cur ); 
            cur = getc( stdin ); 
        }
    }
};

char consumeString( Buffer b, char first ) {

    Buffer_reset( b ); 
    Buffer_write( b, first); 

    char cur = getc( stdin );

    while( true ) {
        if( cur == '\n' || cur == EOF ) {
            ERROR_COUNT++;
            fprintf( stderr, "Error at line %d: Unterminated String\n\t>>%s\n\n", LINE_COUNT, b->stack );
            return cur;
        } 
        if( cur == '"' ) {
            Buffer_write( b, cur ); 
            Buffer_emit( "STRNG", "", b, false );
            return getc( stdin ); 
        }

        Buffer_write( b , cur );
        cur = getc( stdin );
        
    }
};


char consumeComment( Buffer b ) {
    //This consume is non standard as it does not come from the main loop
    //but instead from the operator consume which identifies the opening of the comment. 
    //In this case DO NOT reset the buffer. 

    while( true ) {
        char cur = getc( stdin );     
        if ( cur == '\n' || cur == EOF ) {
            Buffer_emit( "CMNT1", "", b, true );
            return cur;
        }
        Buffer_write( b, cur ); 
    }
};

char consumeMulitlineComment( Buffer b ) {
    //This consume is non standard as it does not come from the main loop
    //but instead from the operator consume which identifies the opening of the comment. 
    //In this case DO NOT reset the buffer. 

    char this, that; 
    this = getc( stdin ); 
    that = getc( stdin ); 

    if( this == '\n' ) LINE_COUNT++; 
    if( that == '\n' ) LINE_COUNT++; 

    while( true ) {

        if( this == '*' && that == '/' ) {
            Buffer_write( b, this ); 
            Buffer_write( b, that ); 
            Buffer_emit( "CMNT2", "", b, true); 
            return getc( stdin ); 
        }
        Buffer_write( b, this ); 
        this = that; 
        that = getc( stdin ); 
        if( that == '\n' ) LINE_COUNT++; 
    }
}

char consumeOp( Buffer b, char first ) {

 // Every if here should have a return, 
 // otherwise you might break the logic. 

    char second = getc( stdin ); 
    Buffer_reset( b ); 
    Buffer_write( b, first ); 

 // if a garunteed on length op - return. 
    if( charIn( first, "(){}[],;+-*") ){
        Buffer_emit ( "OPSZ1", "", b, false ); 
        return second;
    } 

 // if a valid 2 length non comment
    if( ( charIn(first, "=<>!")  && second == '=' ) 
        || ( first == '|' && second == '|' )
        || ( first == '&' && second == '&' )
    ){
        Buffer_write( b, second ); 
        Buffer_emit ( "OPSZ2", "", b, false );
        return getc( stdin ); 
    }

 // if a one line comment 
    if ( first == '/' && second == '/' ) {
        Buffer_write( b, second );  
        return consumeComment(b); 
    }

 // if a multiline comment
    if ( first == '/' && second == '*' ) {
        Buffer_write( b, second ); 
        return consumeMulitlineComment(b); 
    }

 // if not a 2 char, at this point its a one char. 
    if ( charIn( first, "=></" ) ) {
         Buffer_emit( "OPSZ1", "", b, false );
         return second; 

    }   
  
    ERROR_COUNT++;
    fprintf( stderr, "Error at line %d: Illegal Character\n\t>> %s\n\n", LINE_COUNT, b->stack );
    return second; 
};
#endif
