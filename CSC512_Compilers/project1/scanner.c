#ifndef _SCANNER_C_
#define _SCANNER_C_
#include <stdio.h>
#include <stdlib.h>

const char* reserved_words[12] = {
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
**/

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
} Buffer; 

int Buffer_nchars( Buffer b ) { 
    return b.size - b.index; 
};

void Buffer_write( Buffer b, const char c ) {
    b.stack[++b.index] = c; 
};

int Buffer_hasChar( Buffer b ) {
    return ( Buffer_nchars(b) >= 1 );
};

void Buffer_reset( Buffer b ) { 
    b.index = 0; 
    b.size = 0; 
};

void Buffer_rewind( Buffer b ) { 
    b.index = 0; 
};

void Buffer_emit( Buffer b ) { 
   b.stack[ b.index + 1 ] = '\0';
   printf( "%s\n", b.stack );
};

Buffer token_buffer = { 0, 0, "" };

int isAlpha( const char c ) { 
   return (
       ( c >= 'A' && c <= 'z' ) || c == '_'
   ); 
};

int isNumeric( const char c ) { 
   return ( c>= '0' && c <= '9' );
};


#define CONSUME() { \
      cur = getc(stdin);\
      if( cur == EOF ) \
          continue_loop = 1; \
      continue; \
      }
         

void Print_state() {
    switch( state ) {
    case STATE_NEWTOK :
         printf(" State : STATE_NEWTOK \n");
    break;
    case STATE_IDENT :
         printf(" State : STATE_IDENT  \n");
    break;
    case STATE_NUMBER : 
         printf(" State : STATE_NUMBER  \n");
    break; 
    case STATE_SYMBOL : 
         printf(" State : STATE_SYMBOL  \n");
    break;
    case STATE_STRING : 
         printf(" State : STATE_STRING  \n");
    break;
    case STATE_MSTMT:
         printf(" State : STATE_MSTMT \n");
    break;
    }
}
    
int MAX_ITER = 35; 
int CUR_ITER = 0; 
int main( int argc, char ** argp, char ** envp ) { 

    char cur; 
    int continue_loop = 0; 

    cur = getc( stdin );

    if (  cur == EOF ) {
        fprintf(stderr, "No input read\n"); 
        exit(1); 
    }
  
    while( continue_loop == 0  && CUR_ITER++ < MAX_ITER ) { 
        Print_state();
        printf(" %c\n", cur);

        switch( state ) {
        case STATE_NEWTOK :
            if( isNumeric( cur ) ) { 
                state = STATE_NUMBER;
            } else if( isAlpha( cur ) ) { 
                state = STATE_IDENT;
            } else if( cur == '#' ){
                state = STATE_MSTMT;
            } else if( cur == '"' || cur == '\'' ){
                state = STATE_STRING;
            } else if( charIn( cur, "+-/*()") ) {
                state = STATE_SYMBOL;
            }
        break;
        case STATE_IDENT :

        break;
        case STATE_NUMBER : 

        break; 
        case STATE_SYMBOL : 

        break;
        case STATE_STRING : 
 
        break;
        case STATE_MSTMT:
            Buffer_write( token_buffer, cur );
            if( cur == '\n'){
                state = STATE_NEWTOK;
                Buffer_emit( token_buffer );         
                Buffer_reset( token_buffer ); 
            }
            CONSUME();
        break;
        }
    }
};


#endif
