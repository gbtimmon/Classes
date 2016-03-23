
#ifndef POTATO_C
#define POTATO_C

#define _GNU_SOURCE

#include "socket.h"
#include "potato.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>



Potato Potato_new(int count) {

     Potato p = malloc(sizeof(struct potato_t));

     p->initial_count   = count;
     p->current_count   = count; 
     p->signature_count = 0; 
     p->signature       = NULL;
     return p; 
}

void Potato_free(Potato p){
    free(p->signature); 
    free(p);
}

Potato Potato_print(Potato p){
    Potato_fprint(p, STDOUT_FILENO);
    return p; 
}

Potato Potato_fprint(Potato p, Stream s ){
    dprintf(s, "Trace of potato\n");
    dprintf(s, "%d", p->signature[0] );
    for(int i = 1; i < p->signature_count; i++ ) 
        dprintf(s, ",%d", p->signature[i]);
    dprintf(s, "\n");
    return p; 
}

void Potato_start( int c, int s ) {
    Socket_sendi(s, MSG_TYPE_POTATO);
    Socket_sendi(s, c);
    Socket_sendi(s, c); 
    Socket_sendi(s, 0);
}

void Potato_send( Potato p, int i, int s ) {
    Socket_sendi(s, MSG_TYPE_POTATO);
    Socket_sendi(s, p->initial_count); 
    Socket_sendi(s, p->current_count - 1); 
    Socket_sendi(s, p->signature_count + 1);
    for( int j = 0; j < p->signature_count; j++ ) 
        Socket_sendi(s, p->signature[j]); 
    Socket_sendi(s, i); 
}

Potato Potato_recv(char ** s){

    Potato p = malloc(sizeof(struct potato_t)); 

    p->initial_count   = atoi(strsep( s, "\n" ));
    p->current_count   = atoi(strsep( s, "\n" ));
    p->signature_count = atoi(strsep( s, "\n" ));
    p->signature       = malloc( sizeof(int) * p->signature_count );

    for(int i = 0; i < p->signature_count; i++ ) 
        p->signature[i] = atoi(strsep( s, "\n" ));
    
    return p;
}

#endif
