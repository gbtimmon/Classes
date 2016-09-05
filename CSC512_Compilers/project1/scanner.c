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
            fprintf( stderr, "UNTERMINATED STRING FOUND!! %s\n", b->stack );
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

    fprintf( stderr, "ILLEGAL OPERATION!! %s\n", b->stack );
    return second; 
};

int main( int argc, char ** argp, char ** envp ) { 

    /**
     * File handling 
     **/

    if( argc < 2 ) {
        fprintf(stderr, "Filename input required, no given!\n");
        exit(1);
    }
    int fileInFd  = open( argp[1], O_RDONLY );
    int fileOutFd = open( outFile(argp[1]), O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU); // Memory leak with outFile, but its in main and I am to lazy to fix it. 
 
    dup2( fileInFd, STDIN_FILENO );
    dup2( fileOutFd, STDOUT_FILENO );

    Buffer token_buffer = Buffer_new();

    int continue_loop = 0; 
    char cur = getc( stdin );

    if (  cur == EOF ) {
        fprintf(stderr, "No input read\n"); 
        exit(1); 
    }

    while( true )  
        if( cur == EOF ) 
           exit( 0 );

        else if( isalpha( cur ) || cur == '_' ) 
           cur = consumeIdent( token_buffer, cur );

        else if ( isdigit( cur ) ) 
           cur = consumeNumber( token_buffer, cur ); 

        else if ( cur == '#' )
           cur = consumeMeta( token_buffer, cur );

        else if( charIn(cur, "(){}[],;+-*/<>=|&!") ) 
           cur = consumeOp( token_buffer, cur );

        else //Hey this is probably whitespace, just skip it. 
           cur = getc( stdin );
};


#endif
