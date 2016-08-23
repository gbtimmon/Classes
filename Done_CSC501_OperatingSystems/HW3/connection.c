#ifndef CONNECTION_C
#define CONNECTION_C

#define _GNU_SOURCE
#include "socket.h"
#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Connection Connection_new( const char * h, int p, int t, int n){
    Connection c = malloc( sizeof ( struct connection ) ); 
    c->port = p;
    c->type = t; 
    c->num  = n; 
    if( h ) c->host = malloc( sizeof( char* ) * strlen(h) ); 
    if( h ) strcpy( c->host, h); 
    return c; 
}

void Connection_free( Connection c ){
    if( c->host ) free(c->host); 
    free(c); 
    return; 
}


void Connection_send( Connection connection, int socket ) {
    Socket_sendi(socket, MSG_TYPE_CONNECTION);
    Socket_sendi(socket, connection->type); 
    Socket_sends(socket, connection->host);
    Socket_sendi(socket, connection->port);
    Socket_sendi(socket, connection->num); 
}

Connection Connection_recv(char ** s){
     int type = atoi(strsep(s,"\n"));
     char* host = strsep(s,"\n");
     int port = atoi(strsep(s,"\n"));
     int num  = atoi(strsep(s, "\n")); 
     return Connection_new( host, port, type, num); 
}
#endif
