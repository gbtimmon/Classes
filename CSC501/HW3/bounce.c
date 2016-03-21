/******************************************************************************
 *
 *  File Name........: listen.c
 *
 *  Description......:
 *    Creates a program that establishes a passive socket.  The socket 
 *  accepts connection from the speak.c program.  While the connection is
 *  open, listen will print the characters that come down the socket.
 *
 *  Listen takes a single argument, the port number of the socket.  Choose
 *  a number that isn't assigned.  Invoke the speak.c program with the 
 *  same port number.
 *
 *  Revision History.:
 *
 *  When    Who         What
 *  09/02/96    vin         Created
 *
 *****************************************************************************/

/*........................ Include Files ....................................*/
#define _GNU_SOURCE

#include "connection.h"
#include "socket.h"
#include "potato.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int _perror(const char * x, int y){
    perror(x); 
    exit(y); 
}

int rdMsg( int p, char* buf, unsigned int sz, int flags){
    int len =  recv(p, buf, 32, 0);
    if ( len < 0 ) _perror("recv", 1);
    buf[len] = '\0';
    return len; 
}

int getConnection( int s ){
    struct sockaddr_in i; 
    unsigned int l = sizeof(struct sockaddr);
    int p = accept(s, (struct sockaddr *) &i, &l);
    if ( p < 0 ) 
        _perror("accept", p);

    struct hostent* ihp = gethostbyaddr(&i.sin_addr, sizeof(struct in_addr), AF_INET);
    printf(">> Connected to %s\n", ihp->h_name);
    
    return p; 
}


#define BUFFER 512

char       host[BUFFER]; 
int        id; 

int main (int argc, char *argv[])
{
    /* read port number from command line */
    if ( argc < 3 ) {
        fprintf(stderr, "Usage: <me> <you> <num>\n");
        exit(1);
    }

    fprintf(stdout, "me : %s\n", argv[1]); 
    fprintf(stdout, "you: %s\n", argv[2]); 
    fprintf(stdout, "num: %s\n", argv[3]); 
 
    id = atoi( argv[1] ); 
    gethostname( host, BUFFER ); 

    Connection conn_i = Connection_new(host, id ); 
    Connection conn_l = Connection_new(host, atoi(argv[2])); 
    Connection conn_m = Connection_new(host, id); 
    Connection conn_r = Connection_new(host, id); 


    if( atoi(argv[3]) > 0 ) {
        
        int socket = SocketWriter_new( conn_l );
        
        if( socket < 0 )  exit(1); 

        Potato potato = Potato_new( atoi( argv[3] ) );
        Potato_send( potato, id, socket ); 
        Potato_free( potato ); 
        close( socket ); 
    }
  
    int socket_in = SocketListener_new( conn_i ); 
    while (1) { //foreach connection. 

        int   f = getConnection( socket_in );
        char* s = Socket_recv( f ); 
        close(f); 
        char* t = s;
        int   v = atoi(strsep(&t, "\n")); 

        if( v == MSG_TYPE_POTATO ) { 
            Potato p = Potato_recv( &t ); 
            free(s); 
            Potato_print( p ); 
                    
            if( p->current_count == 0 ){
                printf("DONE!\n"); 
                exit(0);
            }

            int socket_out = SocketWriter_new( conn_l );
            Potato_send( p, id, socket_out );
            Potato_free( p ); 
            close( socket_out ); 

        } else { 
             printf("Im confused\n"); 
             close( f ); 
             break;
        }
    }

    Connection_free(conn_i); 
    Connection_free(conn_l); 
    Connection_free(conn_r); 
    Connection_free(conn_m); 
    exit(0);
}

