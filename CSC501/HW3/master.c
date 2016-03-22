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

int main ( int argc, char** argv, char** envv) { 
     
    if( argc < 4 ) {
        fprintf(stderr, "%s: <port> <childcount> <swapcount>\n", argv[0]);
        exit(1); 
    }

    char host[1024];
    gethostname(host, 1024); 

    int port   = atoi(argv[1]); 
    int nchild = atoi(argv[2]); 
    int nswap  = atoi(argv[3]); 
    int cchild = 0; 

    Connection child[ nchild ];
    
    Connection c = Connection_new( host, port, CONN_TYPE_IN);
    int sin = SocketListener_new( c ); 
    Connection_free(c); 
    
    while( 1 ) { 

        char* s = Socket_get_message( sin );
        char* t = s;
        int   v = atoi(strsep(&t, "\n"));

        switch( v ) { 
            case 




        }
        if( v == MSG_TYPE_POTATO ) {
            printf("I got the winner\n"); 
            exit(0); 
        } else if( v == MSG_TYPE_CONNECTION ) { 
            Connection c = Connection_recv( &t ); 
            if( c != CONN_TYPE_CHILD ) {
                free(s);
                Connection_free(c); 
                fprintf(stderr,"I got garbage!\n"); 
                continue; 
            } else {
                //Load the new connection to the arrat
            }
                

  
        }
        free(s);
    }


    }
   
}
#endif
