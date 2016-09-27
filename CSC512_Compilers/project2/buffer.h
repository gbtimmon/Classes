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
    char stack[512];
} * Buffer; 

int    Buffer_nchars  ( Buffer ) ; 
void   Buffer_write   ( Buffer , const char ) ;
int    Buffer_hasChar ( Buffer ) ;
void   Buffer_reset   ( Buffer ) ; 
void   Buffer_rewind  ( Buffer ) ; 
void   Buffer_emit    ( const char*, const char*, Buffer, int ) ; 
Buffer Buffer_new     ( ) ;
int    Buffer_eq      ( Buffer, const char * );
void   Buffer_free    ( Buffer );

#endif
