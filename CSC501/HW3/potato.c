
#ifndef POTATO_C
#define POTATO_C

#define _GNU_SOURCE
#define OK     0
#define NOT_OK 1
#define BUFFER 512

#define STRING_EQ(x,y)  !strcmp(x,y)
#define STRING_NEQ(x,y) strcmp(x,y) 


#include "potato.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


Potato Potato_cook(int count) {

     Potato p = malloc(sizeof(struct potato_t));

     p->initial_count   = count;
     p->current_count   = count; 
     p->signature_count = 0; 
     p->signature       = NULL;
     return p; 
}

Potato Potato_print(Potato p, Stream s ){

    dprintf(s, "potato\n");
    dprintf(s, "%d",  p->initial_count);
    dprintf(s, ",%d", p->current_count);
    dprintf(s, ",%d", p->signature_count);
    for(int i = 0; i < p->signature_count; i++ ) 
        dprintf(s, ",%d", p->signature[i]);
    return p; 
}

String Potato_give( Potato p, int id ) {
    return "asdf";
}

String Stream_readline(Stream stream){

    char   current[1];
    char   buffer[BUFFER] ="" ;
    int    offset         = 0;
    int    char_remaining = 1; 

    while ( char_remaining ) {
        read(stream, &current, 1);

        if( *current == '\n' ) 
            break; 

        buffer[offset++] = *current; 
        ioctl(stream, FIONREAD, &char_remaining);
    }

    String s = malloc(sizeof(char) * ( strlen(buffer) + 1 ) );
    memset(s, '\0', sizeof(char) * ( strlen(buffer) + 1 ) ); 
    strncpy(s, buffer, strlen(buffer));
    return s;
}

Potato Potato_take(Stream stream){
    const char* delim = ",";
    String s = Stream_readline(stream); 

    Potato p = malloc(sizeof(struct potato_t)); 

    p->current_count   = atoi( strsep(&s, delim) );
    p->initial_count   = atoi( strsep(&s, delim) );
    p->signature_count = atoi( strsep(&s, delim) ); 
    p->signature       = malloc( sizeof(int) * p->signature_count );

    for(int i = 0; i < p->signature_count; i++ ) 
        p->signature[i] = atoi( strsep(&s, delim) ); 
    
    free(s); 
    return p;
}

void Potato_free(Potato p){
    free(p->signature); 
    free(p);
}
#endif
