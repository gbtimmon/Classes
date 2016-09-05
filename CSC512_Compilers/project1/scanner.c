#ifndef _SCANNER_C_
#define _SCANNER_C_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

typedef FILE* File; 
#define RESERVED_WORD_COUNT 13
const char* reserved_word[ RESERVED_WORD_COUNT ] = {
    "main",
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

/**
    STATES
    ----------------------
    These are the states that the scanner can occupy
    this is the hiearchy tree

    <identifier>
        |
        +-<reserved word>

    <number>

    <symbol>

    <string>

    <meta statement>
*/

#define STATE_NEWTOK  -1
#define STATE_IDENT   0
#define STATE_RWORD   1
#define STATE_NUMBER  2
#define STATE_SYMBOL  3
#define STATE_STRING  4
#define STATE_MSTMT   5

// We start with no token. 
int state = STATE_NEWTOK;

/**   
    BUFFER 
    ----------------------
    This buffer will record a "memory of the tokens seen
    When a regex fails, we will roll back to the last known 
    breakpoint and try again with the next option in the chain. 
    If a token ends on a valid terminating state, it will clear
    and emit the token to output. 

    You can 
       write to the buffer, 
       read the next char from the buffer
       rewind the buffer - start at the beggining without clearing the memory
       reset the buffer - start at the beggining with clearing the memory
**/
typedef struct _buffer {
    int index; 
    int size; 
    char stack[1024];
} * Buffer; 

int Buffer_nchars( Buffer b ) { 
    return b->size - b->index; 
};

void Buffer_write( Buffer b, const char c ) {
    b->stack[b->size++] = ( c == '\n') ? ' ' : c; 
};

int Buffer_hasChar( Buffer b ) {
    return ( Buffer_nchars(b) >= 1 );
};

void Buffer_reset( Buffer b ) { 
    b->index = 0; 
    b->size = 0; 
    memset( b->stack, '\0', sizeof b->stack);
};

void Buffer_rewind( Buffer b ) { 
    b->index = 0; 
};

void Buffer_emit( const char* state, const char* prefix, Buffer b, int newLine ) { 
   b->stack[ b->size + 1 ] = '\0';
   printf( "[%s] %s%s%s", state, prefix,  b->stack, (newLine) ? "\n" : "\n" );
};

Buffer Buffer_new() {
    Buffer n = (Buffer) malloc(sizeof(_buffer));
    n->size = 0;   
    n->index = 0; 
    memset( n->stack, '\0', sizeof n->stack);
    return n;
};

int isReserved( Buffer b ) {
    for ( int i = 0; i < RESERVED_WORD_COUNT; i++ ) {
        if( strcmp( reserved_word[ i ] , b->stack ) == 0 )
            return 1; 
    }
    return 0;
};

const char * stateString() {
    switch( state ) {
    case STATE_NEWTOK :
         return "STATE_NEWTOK";
    break;
    case STATE_IDENT :
          return "STATE_IDENT";
    break;
    case STATE_NUMBER : 
          return "STATE_NUMBER";
    break; 
    case STATE_SYMBOL : 
          return "STATE_SYMBOL";
    break;
    case STATE_STRING : 
          return "STATE_STRING";
    break;
    case STATE_MSTMT:
          return "STATE_MSTMT";
    break;
    }
}

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

int quotes_seen = 0; /* Im cheating here to use a global for string recognition. 
                        I should probably put token processing in localized subfunctions. 
                        
                        But Im to lazy since this is just a homework that wont be expanded on greatly. 
                      */

char consumeIdent( Buffer b, File stream, char first ) {

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
        cur = getc( stream );
    }
}
     
char consumeNumber( Buffer b, File stream, char first ){

    char cur = first; 
    Buffer_reset( b ); 

    while( true ) {
        if( isdigit( cur ) ) {
            Buffer_write( b, cur);
        } else {
            Buffer_emit( "NUMBR", "", b, false);         
            return cur; 
        }
        cur = getc( stream );
    }
};

char consumeMeta( Buffer b, File stream, char first ) {

    char cur = first; 
    Buffer_reset( b ); 

    while( true ) {
        if( cur == '\n'){
            Buffer_emit( "MSTMT", "", b, true );         
            return getc( stream ); 
        } else {
            Buffer_write( b, cur ); 
            cur = getc( stream ); 
        }
    }
};

char consumeString( Buffer b, File stream, char first ) {

    Buffer_reset( b ); 
    Buffer_write( b, first); 

    char cur = getc(stream);; 

    while( true ) {
        if( cur == '\n' || cur == EOF ) {
            fprintf( stderr, "UNTERMINATED STRING FOUND!! %s\n", b->stack );
            return cur;
        } 
        if( cur == '"' ) {
            Buffer_write( b, cur ); 
            Buffer_emit( "STRNG", "", b, false );
            return getc( stream ); 
        }

        Buffer_write( b , cur );
        cur = getc( stream );
        
    }
};

char consumeOp( Buffer b, File stream, char first ) {

    char cur = first;
    Buffer_reset( b ); 

    while( true ) {

        if( b->size == 0 ) {
            if( charIn( cur, "(){}[],;+/-*") ){
                Buffer_write( b, cur ); 
                Buffer_emit ( "OPSZ1", "", b, false ); 
                return getc( stream );
            } else if ( charIn( cur, "=><!|&" ) ){
                Buffer_write( b, cur );
                cur = getc( stream ); 
            }
        } else {
            char prev = b->stack[0];
            if(  ( charIn(prev, "=<>!")  && cur == '=' ) 
                 || ( prev == '|' && cur == '|' )
                 || ( prev == '&' && cur == '&' )
            ){
                 Buffer_write( b, cur );
                 Buffer_emit ( "OPSZ2", "", b, false );
                 return getc( stream ); 

            } else if ( charIn( prev, "=><" ) ) {
                 Buffer_emit( "OPSZ1", "", b, false );
                 return cur; 
            }  else {
                 fprintf( stderr, "ILLEGAL OPERATION!! %s\n", b->stack );
                 return cur; 
            }
        }
    }
};

int main( int argc, char ** argp, char ** envp ) { 

    /**
     * File handling 
     **/

    if( argc < 2 ) {
        fprintf(stderr, "Filename input required, no given!\n");
        exit(1);
    }
    File fileIn  = fopen( argp[1], "r" );
    File fileOut = fopen( outFile(argp[1]), "w" ); // Memory leak, but its in main and I am to lazy to fix it. 

    Buffer token_buffer = Buffer_new();

    int continue_loop = 0; 
    char cur = getc( fileIn );

    if (  cur == EOF ) {
        fprintf(stderr, "No input read\n"); 
        exit(1); 
    }

    while( true )  
        if( cur == EOF ) 
           exit( 0 );

        else if( isalpha( cur ) || cur == '_' ) 
           cur = consumeIdent( token_buffer, fileIn, cur );

        else if ( isdigit( cur ) ) 
           cur = consumeNumber( token_buffer, fileIn, cur ); 

        else if ( cur == '#' )
           cur = consumeMeta( token_buffer, fileIn, cur );

        else if( charIn(cur, "(){}[],;+-*/<>=|&!") ) 
           cur = consumeOp( token_buffer, fileIn, cur );

        else 
           cur = getc( fileIn );
};


#endif
