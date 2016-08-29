#ifndef _SCANNER_C_
#define _SCANNER_C_
#include <stdio.h>

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

void Buffer_read( Buffer b, const char c ) {
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

Buffer token_buffer = { 0, 0, "" };
//I MAY NOT USE THE BUFFER. 
// WELL SEE.....


int isAlpha( const char c ) { 
   return (
       ( c >= 'A' && c <= 'z' ) || c == '_'
   ); 
};

int isNumeric( const char c ) { 
   return ( c>= '0' && c <= '9' );
};


int main( int argc, char ** argp, char ** envp ) { 

    char buffer [1024], 
         cur; 

    while( ( cur = getc( stdin ) ) != EOF ) { 
    
        printf(" %c\n", cur);

        switch( state ) {
        case STATE_NEWTOK :
  
        break;
        case STATE_IDENT :

        break;
        case STATE_NUMBER : 

        break; 

   
        }



       
        

    }
};


#endif
