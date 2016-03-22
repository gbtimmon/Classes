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


#define DEFAULT_PORT 9999

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

#define    HBUFF                512
char       player_host[HBUFF] = "\0";
char       master_host[HBUFF] = "\0";
int        id                 = 11;

int main (int argc, char *argv[])
{
    /* read port number from command line */
    if ( argc < 3 ) {
        fprintf(stderr, "Usage: <master-machine-name> <master-port-number>\n");
        exit(1);
    }

    gethostname( player_host, HBUFF ); 
    strcpy(master_host, argv[1]);

    Connection conn_in = Connection_new(player_host, DEFAULT_PORT, CONN_TYPE_IN ); 


    printf("I am listening on port %d!\n", conn_in->port); 
    Connection conn_r = Connection_new(player_host, DEFAULT_PORT, CONN_TYPE_RIGHT); 
    Connection conn_m = Connection_new(player_host, DEFAULT_PORT, CONN_TYPE_IN); 
    Connection conn_l = Connection_new(player_host, DEFAULT_PORT, CONN_TYPE_LEFT); 

  
    int socket_in = SocketListener_new( conn_in ); 
    while (1) { //foreach connection. 

        char* s = Socket_get_message( socket_in );
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
             break;
        }
        free(s); 
    }

    Connection_free(conn_in); 
    Connection_free(conn_l); 
    Connection_free(conn_r); 
    Connection_free(conn_m);
    exit(0);
}

