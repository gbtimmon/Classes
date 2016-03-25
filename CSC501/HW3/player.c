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

#include "random.h"
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


#define DEFAULT_PORT  9999
#define DEFAULT_PORT2 99999
#define HBUFF         512

char       player_host[HBUFF] = "\0";
char       master_host[HBUFF] = "\0";
int        id                 = -1;

void reportAddress( Connection reportWhat, Connection reportTo ){

     int socket_mast = SocketWriter_new( reportTo );
     Connection myAddressReport = Connection_new( reportWhat->host, reportWhat->port, CONN_TYPE_CHILD, -1);
     Connection_send( myAddressReport, socket_mast );
     Connection_free( myAddressReport );
     close( socket_mast );

}

void passPotato( Potato passWhat, Connection passTo ) {

     int socket_out = SocketWriter_new( passTo );
     Potato_send( passWhat, id, socket_out );
     close(socket_out);   

}

int main (int argc, char *argv[])
{
    /* read port number from command line */
    if ( argc < 3 ) {
        fprintf(stderr, "Usage: <master-machine-name> <master-port-number>\n");
        exit(1);
    }

    gethostname( player_host, HBUFF ); 
    strcpy(master_host, argv[1]);

    int defaultPort = ( DEFAULT_PORT != atoi(argv[2])) ? DEFAULT_PORT : DEFAULT_PORT2; 

    Connection conn_in = Connection_new(player_host, defaultPort, CONN_TYPE_IN, -1 ); 
    int socket_in = SocketListener_new( conn_in ); 

    Connection conn_m = Connection_new(master_host, atoi(argv[2]), CONN_TYPE_MASTER, -1); 
    reportAddress( conn_in, conn_m );
    
    Connection conn_r = NULL;
    Connection conn_l = NULL;

  
    while (1) { //foreach connection. 

        char* s = Socket_get_message( socket_in );
        char* t = s;
        int   v = atoi(strsep(&t, "\n")); 

        switch (v ) { 
            case MSG_TYPE_POTATO :
            {
                if ( conn_l == NULL || conn_r == NULL ) {
                    fprintf(stderr, "I recieved a potatoe too soon I an not initalized!\n");
                    exit(1); 
                }

                Potato p = Potato_recv( &t ); 
                Connection targ;
                if( p->current_count == 0 ){
                     printf("I'm it\n");
                     targ = conn_m;
                } else if ( idrandom( id ) % 2 ) {
                     targ = conn_r; 
                     printf("Sending potato to %d\n", targ->num);
                } else {
                     targ = conn_l; 
                     printf("Sending potato to %d\n", targ->num);
                }
                passPotato(p, targ);
                Potato_free( p ); 
                break;
            }
            case MSG_TYPE_CONNECTION :
            {
                Connection c = Connection_recv( &t );
                if( c->type == CONN_TYPE_LEFT )
                    conn_l = c; 
                else if (c->type == CONN_TYPE_RIGHT )
                    conn_r = c; 
                break;
            }
            case MSG_TYPE_CHILD_NAME :
            {
                id = atoi(strsep( &t, "\n") );
                printf("Connected as player %d\n", id);
                break;
            }
            case MSG_TYPE_CLOSE : 
            {
                exit(0);
                break;
            }
        }
        free(s); 

    }

    Connection_free(conn_in); 
    Connection_free(conn_l); 
    Connection_free(conn_r); 
    Connection_free(conn_m);
    exit(0);
}

