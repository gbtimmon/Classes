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
                     childv[childc] = Connection_recv( &t );
                     Connection_print(childv[childc]);
                     fprintf(stdout, "FIXME!! : Connected to %s on port %d\n", childv[childc]->host, childv[childc]->port); 
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
