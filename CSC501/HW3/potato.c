
#ifndef POTATO_C
#define POTATO_C

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


Potato Potato_cook(int count) {
     Potato p = malloc(sizeof(struct potato_t));
     p->initial_count = count;
     p->current_count = count; 
     p->signature = NULL;
     return p; 
}

Potato Potato_sign(Potato p, int id){
    Signature s = malloc(sizeof(struct signature_t)); 
    s->id       = id; 
    //s->hostname = malloc(sizeof(char) * ( strlen(hostname) + 1 ) ); 
    s->next     = NULL;
    //strncpy(s->hostname, hostname, strlen(hostname));    

    if(! p->signature) {
        p->signature = s; 
        return p;
    } else {
        Signature t = p->signature; 
        while(t->next)
            t = t->next; 
        t->next = s; 
        return p;
    }
}

Potato Potato_give(Potato p, int stream){
    dprintf(stream, "%d", p->initial_count);
    dprintf(stream, ",%d", p->current_count);
    Signature s = p->signature; 
    while(s){
        dprintf(stream, ",%d", s->id);
        s = s->next; 
    }
    dprintf(stream, "\n");
    return p; 
}

String _readline(Stream stream){

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
    strncpy(s, buffer, strlen(buffer));
    return s;
}

Potato Potato_take(Stream stream){
    const char* delim = ",";
    Potato p; 

    String s = _readline(stream); 

    p = malloc(sizeof(struct potato_t)); 
    p->current_count = atoi( strsep(&s, delim) );
    p->initial_count = atoi( strsep(&s, delim) );
    
    return p;
}

void free_sig(Signature s) {
    if(s == NULL) return; 
    free_sig(s->next);
    //free(s->hostname);
    free(s);
    return;
}

Potato Potato_free(Potato p){
    free_sig(p->signature);
    free(p);
}
#endif
