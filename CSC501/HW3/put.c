/******************************************************************************
 *
 *  File Name........: speak.c
 *
 *  Description......:
 *	Create a process that talks to the listen.c program.  After 
 *  connecting, each line of input is sent to listen.
 *
 *  This program takes two arguments.  The first is the host name on which
 *  the listen process is running. (Note: listen must be started first.)
 *  The second is the port number on which listen is accepting connections.
 *
 *  Revision History.:
 *
 *  When	Who         What
 *  09/02/96    vin         Created
 *
 *****************************************************************************/

/*........................ Include Files ....................................*/
#define _GNU_SOURCE

#include "potato.h"
#include "socket.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LEN	64
 
int main (int argc, char *argv[])
{
    /* read host and port number from command line */
    if ( argc != 3 ) {
        fprintf(stderr, "Usage: %s <host-name> <port-number>\n", argv[0]);
        exit(1);
     }
  
    int s = SocketWriter_new( argv[1], atoi(argv[2]) ); 
    if( s < 0 ) exit(EXIT_FAILURE); 

    /* read a string from the terminal and send on socket */
    Potato p = Potato_new(1);
    String str = Potato_toString(p, 1);
    printf("[[%s]]\n", str); 
    Potato_free(p); 

    int  len = send(s, str, strlen(str), 0);
    if ( len != strlen(str) ) {
        perror("send");
        exit(1);
    }

    len = send(s, "close", 5, 0);
    if (len != 5) {
        perror("send");
        exit(1);
    }
    close(s);
    exit(0);
}

/*........................ end of speak.c ...................................*/
