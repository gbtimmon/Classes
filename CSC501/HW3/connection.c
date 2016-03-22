#ifndef CONNECTION_C
#define CONNECTION_C

#include "socket.h"
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


void Connection_send( Connection connection, int target, int socket ) {
    Socket_sendi(socket, MSG_TYPE_CONNECTION);
    Socket_sendi(socket, target); 
    Socket_sends(socket, connection->host);
    Socket_sendi(socket, connection->port);
}

void Connection_recv(char ** s){

    Connection c = malloc(sizeof(struct connection)); 

    p->initial_count   = atoi(strsep( s, "\n" ));
    p->current_count   = atoi(strsep( s, "\n" ));
    p->signature_count = atoi(strsep( s, "\n" ));
    p->signature       = malloc( sizeof(int) * p->signature_count );

    for(int i = 0; i < p->signature_count; i++ )
        p->signature[i] = atoi(strsep( s, "\n" ));

    return p;
}


#endif
