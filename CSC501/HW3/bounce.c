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

    //struct hostent* ihp = gethostbyaddr(&incoming.sin_addr, sizeof(struct in_addr), AF_INET);
    //printf(">> Connected to %s\n", ihp->h_name);
    
    return p; 
}


#define BUFFER 256

int si; 
int id; 

char host[BUFFER]; 

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
    int si = SocketListener_new( atoi(argv[1]));
    if( si < 0 ) exit(1); 

    if( atoi(argv[3]) > 0 ) {
        int s = SocketWriter_new( host, atoi(argv[2]));
        Potato p = Potato_new( atoi(argv[3]) );
        Potato_give( p, id, s ); 
        Potato_free(p); 
        close(s); 
     }
  
    char buf[512];
    while (1) { //foreach connection. 
        int connection = getConnection(si);
        while ( 1 ) {
            if ( !rdMsg(connection, buf, 64, 0) ) break;
            if ( !strcmp("close", buf) ) break;

            printf("MSG > %s\n", buf);

            int s = SocketWriter_new( host, atoi(argv[2]));
            Potato p = Potato_take(buf); 
            Potato_give( p, id, s );
            Potato_free(p); 
            close(s);
        }
        close(connection);
        printf(">> Connection closed\n");
    }
    exit(0);
}

