#ifndef CONNECTION_C
#define CONNECTION_C

#include "connection.h"

#include <stdlib.h>
#include <string.h>

Connection Connection_new( const char * h, int p){
    Connection c = malloc( sizeof ( struct connection ) ); 
    c->port = p;
    if( h ) c->host = malloc( sizeof( char* ) * strlen(h) ); 
    if( h ) strcpy( c->host, h); 
    return c; 
}

void Connection_free( Connection c ){
    if( c->host ) free(c->host); 
    free(c); 
    return; 
}

#endif
