#ifndef CONNECTION_C
#define CONNECTION_C

#include "socket.h"
#include "connection.h"

#include <stdlib.h>
#include <string.h>

Connection Connection_new( const char * h, int p, int t){
    Connection c = malloc( sizeof ( struct connection ) ); 
    c->port = p;
    c->type = t; 
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
    Socket_sendi(socket, connection->type); 
    Socket_sends(socket, connection->host);
    Socket_sendi(socket, connection->port);
}

void Connection_recv(char ** s){
     int type = atoi(strsep(s,"\n"));




     return Connection_new( 
}


#endif
