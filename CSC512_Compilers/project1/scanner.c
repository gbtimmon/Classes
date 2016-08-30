#ifndef _SCANNER_C_
#define _SCANNER_C_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void Buffer_emit( Buffer b ) { 
   b->stack[ b->size + 1 ] = '\0';
   printf( "EMIT : [%s]\n", b->stack );
};

Buffer Buffer_new() {
    Buffer n = (Buffer) malloc(sizeof(_buffer));
    n->size = 0;   
    n->index = 0; 
    memset( n->stack, '\0', sizeof n->stack);
    return n;
};
    
Buffer token_buffer = Buffer_new();


#define CONSUME() { \
      cur = getc(stdin);\
      if( cur == EOF ) \
          continue_loop = 1; \
      continue; \
      }
         

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
   
void keywordCheck( ) { 
    printf("Key word check not implemented\n"); 
}

int MAX_ITER = 1000; 
int CUR_ITER = 0; 
int quotes_seen = 0;
int main( int argc, char ** argp, char ** envp ) { 

    char cur; 
    int continue_loop = 0; 

    cur = getc( stdin );

    if (  cur == EOF ) {
        fprintf(stderr, "No input read\n"); 
        exit(1); 
    }
  
    while( continue_loop == 0  && CUR_ITER++ < MAX_ITER ) { 

        switch( state ) {
        case STATE_NEWTOK :
            if( isdigit( cur ) ) { 
                state = STATE_NUMBER;
            } else if( isalpha( cur ) || cur == '_' ) { 
                state = STATE_IDENT;
            } else if( cur == '#' ){
                state = STATE_MSTMT;
            } else if( cur == '"' /*|| cur == '\''*/ ){
                state = STATE_STRING;
            } else if( charIn( cur, "+-/*()") ) {
                state = STATE_SYMBOL;
            } else {
                CONSUME();
            }
            printf("I see a %s\n", stateString()) ;
               
        break;
        case STATE_IDENT :
            if( isalpha(cur) || isdigit(cur) || cur == '_' ){
                Buffer_write( token_buffer, cur );
                CONSUME();
            } else {
                keywordCheck();
                Buffer_emit( token_buffer );
                Buffer_reset( token_buffer); 
                state = STATE_NEWTOK;
            }
        case STATE_NUMBER : 
            if( isdigit( cur ) ) {
                Buffer_write( token_buffer, cur);
                CONSUME();
            } else {
                Buffer_emit( token_buffer );         
                Buffer_reset( token_buffer ); 
                state = STATE_NEWTOK;
            }
        break; 
        case STATE_SYMBOL : 
            state = STATE_NEWTOK;
            CONSUME();

        break;
        case STATE_STRING : 
            if( cur == '"' ) {
                quotes_seen++;
                if( quotes_seen == 2){
                    quotes_seen = 0; 
                    Buffer_emit( token_buffer );
                    Buffer_reset( token_buffer ); 
                    state=STATE_NEWTOK;
                    continue;
                }
            }
            Buffer_write( token_buffer, cur );
            CONSUME();
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
