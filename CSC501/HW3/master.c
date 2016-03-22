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

void nameChild( Connection child, int name ) {
    int sock = SocketWriter_new( child );
    Socket_sendi( sock, MSG_TYPE_CHILD_NAME );
    Socket_sendi( sock, name );
    close(sock);    
}

void connectChildren(int childc, Connection* childv ){
    printf("connect!\n");
}

void startGame( Connection starter, int swapn ){ 
    printf("Starting!\n");
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
    Connection_free(c); 
    
    while( 1 ) { 

        char* s = Socket_get_message( sin );
        char* t = s;
        int   v = atoi(strsep(&t, "\n"));

        switch( v ) { 
            case MSG_TYPE_POTATO :
                printf("I got the winner\n");
                exit(0);
                break;
            case MSG_TYPE_CONNECTION :
                if( childn == childc) {
                     fprintf(stderr,"%d connections already recieved, and I recieved annother. Refusing connection\n", childn);
                } else {
                     Connection cc = Connection_recv( &t );
                     childv[childc] = cc;
                     Connection_print(cc);
                     fprintf(stdout, "FIXME!! : Connected to %s on port %d\n", cc->host, cc->port); 
                     nameChild(cc, childc);
                     childc++;
 
                     if( childn == childc) {
                         connectChildren(childn, childv);
                         startGame(childv[0], swapn);
                     }
                }
            break; 
        }
        free(s);
    }
}
#endif
