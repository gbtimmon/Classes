#ifndef MASTER_C
#define MASTER_C

#define _GNU_SOURCE

#include "socket.h"
#include "connection.h"
#include "potato.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void makeConnection( Connection connFrom, Connection connTo, int type ) {
    int sock = SocketWriter_new(connFrom);
    Connection c = Connection_new( connTo->host, connTo->port, type );
    Connection_send( c, sock ); 
    Connection_free( c ); 
    close(sock); 
}

void nameChild( Connection child, int name ) {
    int sock = SocketWriter_new( child );
    Socket_sendi( sock, MSG_TYPE_CHILD_NAME );
    Socket_sendi( sock, name );
    close(sock);    
}

void connectChildren(int childn, Connection* childv ){
    for( int i = 0; i < childn; i++ ) {
       int offset = ( i + 1 ) % childn; 
       makeConnection(childv[i], childv[offset], CONN_TYPE_LEFT);
    }

    for( int i = 0; i < childn; i++ ) {
       int offset = ( i - 1 ) % childn; 
       if( offset == -1 ) offset = childn - 1;
       makeConnection(childv[i], childv[offset], CONN_TYPE_RIGHT);
    }
}

void startGame( int childn, Connection * childv, int swapn ){ 
    int offset = random() % childn;
    printf("All players present, sending potato to player %d\n", offset + 1); 
    Connection starter = childv[offset];
    int sock = SocketWriter_new( starter ); 
    Potato_start( swapn, sock ); 
    close( sock );        
}

void closeChildren( int childc, Connection* childv) {
    for( int i = 0; i < childc; i++ ) {
        int sock = SocketWriter_new( childv[i] );
        Socket_sendi( sock, MSG_TYPE_CLOSE );
        close(sock);
    }
}

int main ( int argc, char** argv, char** envv) { 
     
    if( argc < 4 ) {
        fprintf(stderr, "%s: <port> <childcount> <swapcount>\n", argv[0]);
        exit(1); 
    }

    char host[1024];
    gethostname(host, 1024); 

    int port   = atoi(argv[1]); 
    int childn = atoi(argv[2]); 
    int swapn  = atoi(argv[3]); 
    int childc = 0; 

    Connection childv[ childn ];
    
    Connection c = Connection_new( host, port, CONN_TYPE_IN);
    int sin = SocketListener_new( c ); 
   
    if( c->port != port ) {
       fprintf(stderr, "Master port unavailable, please try again.\n");
       exit(1);
    }
  
    Connection_free(c); 
    
    printf("Potato Master on %s\n", host);
    printf("Players = %d\n", childn);
    printf("Hops = %d\n", swapn); 
    while( 1 ) { 

        char* s = Socket_get_message( sin );
        char* t = s;
        int   v = atoi(strsep(&t, "\n"));
        switch( v ) { 
            case MSG_TYPE_POTATO :
            {
                Potato p = Potato_recv( &t ); 
                Potato_print( p ); 
                Potato_free( p ); 
   
                closeChildren( childc, childv ); 
                exit(0);
                break;
            }
            case MSG_TYPE_CONNECTION :
            {
                if( childn == childc) {
                     fprintf(stderr,"%d connections already recieved, and I recieved annother. Refusing connection\n", childn);
                } else {
                     Connection cc = Connection_recv( &t );
                     childv[childc] = cc;
                     fprintf(stdout, "player %d is on %s\n", childc + 1, cc->host); 
                     nameChild(cc, childc + 1);
                     childc++;
 
                     if( childn == childc) {
                         connectChildren(childn, childv);
                         if(swapn > 0 ){
                             startGame(childn, childv, swapn);
                         } else {
                             closeChildren( childc, childv );
                             exit(0);
                         }
                     }
                }
                break; 
            }
        }
        free(s);
    }
}
#endif
