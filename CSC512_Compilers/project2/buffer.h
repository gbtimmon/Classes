#ifndef _BUFFER_H_
#define _BUFFER_H_

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

int Buffer_nchars( Buffer b ) ; 
void Buffer_write( Buffer b, const char c ) ;
int Buffer_hasChar( Buffer b ) ;
void Buffer_reset( Buffer b ) ; 
void Buffer_rewind( Buffer b ) ; 
void Buffer_emit( const char* state, const char* prefix, Buffer b, int newLine ) ; 
Buffer Buffer_new() ;
void Buffer_free( Buffer b );

#endif
