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

#include "./buffer.h"
#include "./scanner.h"

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
//Local functions.
int charIn( const char, char *);
int isReserved( Buffer b );
char * outFile( const char * inFile );

void consumeIdent( Scanner );
void consumeMeta( Scanner );
void consumeString( Scanner );
void consumeMulitlineComment( Scanner );
void consumeOp( Scanner );
void consumeNumber( Scanner );
void consumeComment( Scanner );

// I want my "Parser" to "have" a scanner, so lets make scanner OOP.
//
// Changes from last time -- We need to:
//    1.) take our redirects and make them non global. 
//    2.) store state so I can demand a token as they are needed by the object owner. 
//    4.) Implement getNext()
//    5.) Implement constructor destructor. 
//    6.) Implement objects for terminals (structs with label (Token)) 

Scanner Scanner_new( const char * inFile, const char * outFile ) {
    Scanner n = malloc( sizeof (struct _scanner) );

    n->lineNo  = 0; 
    n->errCnt  = 0;
    n->outFlag = 0;
    n->in      = ( inFile == NULL) ? stdin : fopen( inFile, "r");
    n->out     = ( outFile == NULL)? stdout: fopen(outFile, "w");
    n->buffer  = Buffer_new(); 
    n->token   = T_EMPTY;
    n->cur     = getc( n->in );
}

void Scanner_free( Scanner s ) {
    Buffer_free( s->buffer );
    free(s); 
}

void Scanner_setOutput( Scanner s, int output ) {
    s->outFlag = output;
};

void Scanner_nextChar( Scanner s ) {
    s->cur = getc( s ->in) ;
}
// This will load the Scanner->token data with the next good token. 
// It will overwrite the last token. The return will return the same 
// memory location everytime, so the return is just for synatic sugar. 
Token Scanner_nextToken( Scanner s ) {
   
    if( s->cur == '\n') 
        s->lineNo++;

    // One of these braches will build a new token. 
    // if I get to the final else, I will skip a char and try again. 
    // otherwise ive built a new good token.
    if( s->cur == EOF ) 
        return Token_new( T_EOF, "");
    
    else if( isalpha( s->cur ) || s->cur == '_' ) 
        consumeIdent( s );

    else if ( isdigit( s->cur ) ) 
        consumeNumber( s ); 

    else if ( s->cur == '#' )
        consumeMeta( s );

    else if ( s->cur == '"' )
        consumeString( s );

    else if( charIn(s->cur, "(){}[],;+-*/<>=|&!") ) 
        consumeOp( s );

    else { 
        Scanner_nextChar( s );
        return Scanner_nextToken( s ); 
    } 
    
    return Token_new( s->token, s->buffer->stack ); 
}

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

void consumeIdent( Scanner s ) {

    Buffer_reset( s->buffer );

    while( true ) {
        if( isalpha(s->cur) || isdigit(s->cur) || s->cur == '_' ){
            Buffer_write( s->buffer, s->cur );
        } else {
            if( isReserved ( s->buffer ) ) {
                s->token = T_KEYWORD;
            } else {
                s->token = T_IDENT;
            }
            return;
        }
        Scanner_nextChar( s ); 
    }
}
     

void consumeMeta( Scanner s ) {

    Buffer_reset( s->buffer ); 

    while( true ) {
        if( s->cur == '\n'){
            s->lineNo++;
            s->token = T_META;
            Scanner_nextChar( s ); 
            return; 
        } else {
            Buffer_write( s->buffer, s->cur ); 
            Scanner_nextChar( s ); 
        }
    }
};

void consumeString( Scanner s ) {

    Buffer_reset( s->buffer ); 
    Buffer_write( s->buffer, s->cur); 
    Scanner_nextChar( s ); 

    while( true ) {
        if( s->cur == '\n' || s->cur == EOF ) {
            s->errCnt++;
            fprintf( stderr, "Error at line %d: Unterminated String\n\t>>%s\n\n", 
                          s->lineNo, s->buffer->stack );
            return;
        } 
        if( s->cur == '"' ) {
            Buffer_write( s->buffer, s->cur ); 
            s->token = T_STRING;
            Scanner_nextChar( s ); 
            return; 
        }

        Buffer_write( s->buffer , s->cur );
        Scanner_nextChar( s ); 
    }
};



void consumeMulitlineComment( Scanner s ) {
    //This consume is non standard as it does not come from the main loop
    //but instead from the operator consume which identifies the opening of the comment. 
    //In this case DO NOT reset the buffer. 

    char this = s->cur;
    Scanner_nextChar( s ); 
    char that = s->cur;

    if( this == '\n' ) s->lineNo++; 
    if( that == '\n' ) s->lineNo++; 

    while( true ) {

        if( this == '*' && that == '/' ) {
            Buffer_write( s->buffer, this ); 
            Buffer_write( s->buffer, that ); 
            s->token = T_MCOMMENT;
            Scanner_nextChar( s ); 
            return; 
        }
        Buffer_write( s->buffer, this ); 
        this = that; 
        Scanner_nextChar( s ); 
        that = s->cur;
        if( that == '\n' ) s->lineNo++; 
    }
}

void consumeOp( Scanner s ) {

 // Every if here should have a return, 
 // otherwise you might break the logic. 
    char first  = s->cur;
    Scanner_nextChar( s ); 
    char second = s->cur; 

    Buffer_reset( s->buffer ); 
    Buffer_write( s->buffer, first ); 

 // if a garunteed on length op - return. 
    if( charIn( first, "(){}[],;+-*") ){
        s->token = T_OP;
        return;
    } 

 // if a valid 2 length non comment
    if( ( charIn(first, "=<>!")  && second == '=' ) 
        || ( first == '|' && second == '|' )
        || ( first == '&' && second == '&' )
    ){
        Buffer_write( s->buffer, second ); 
        s->token = T_OP;
        Scanner_nextChar( s ); 
        return; 
    }

 // if a one line comment 
    if ( first == '/' && second == '/' ) {
        Buffer_write( s->buffer, second );  
        Scanner_nextChar( s ); 
        return consumeComment( s ); 
    }

 // if a multiline comment
    if ( first == '/' && second == '*' ) {
        Buffer_write( s->buffer, second ); 
        Scanner_nextChar( s ); 
        return consumeMulitlineComment( s ); 
    }

 // if not a 2 char, at this point its a one char. 
    if ( charIn( first, "=></" ) ) {
        s->token = T_OP;
        return ; 

    }   
  
    s->errCnt++; 
    fprintf( stderr, "Error at line %d: Illegal Character\n\t>> %s\n\n",
                                   s->lineNo, s->buffer->stack );
    return; 
};

void consumeNumber( Scanner s ){

    Buffer_reset( s->buffer ); 

    while( true ) {
        if( isdigit( s->cur ) ) {
            Buffer_write( s->buffer, s->cur);
        } else {
            s->token = T_NUMBER;
            return; 
        }
        Scanner_nextChar( s ); 
    }
};

void consumeComment( Scanner s ) {
    //This consume is non standard as it does not come from the main loop
    //but instead from the operator consume which identifies the opening of the comment. 
    //In this case DO NOT reset the buffer. 

    Buffer_write( s->buffer, s->cur ); 
    while( true ) {
        Scanner_nextChar( s );      
        if ( s->cur == '\n' || s->cur == EOF ) {
            s->token = T_COMMENT;
            return;
        }
        Buffer_write( s->buffer, s->cur ); 
    }
};
#endif
