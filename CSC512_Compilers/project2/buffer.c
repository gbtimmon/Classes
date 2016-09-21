#ifndef _BUFFER_C
#define _BUFFER_C

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
   printf( "%s%s%s", prefix, b->stack, (newLine) ? "\n" : " " );
};

Buffer Buffer_new() {
    Buffer n = (Buffer) malloc(sizeof( struct _buffer));
};


#endif
