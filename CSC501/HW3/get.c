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

int main (int argc, char *argv[])
{
    /* read port number from command line */
    if ( argc < 2 ) {
      fprintf(stderr, "Usage: %s <port-number>\n", argv[0]);
      exit(1);
    }

    int s = SocketListener_new( atoi(argv[1]));
    if( s < 0 ) exit(1); 

    char buf[512];
    char host[64];
    while (1) { //foreach connection. 
      struct sockaddr_in incoming; 
      int len = sizeof(struct sockaddr);
      int p = accept(s, (struct sockaddr *)&incoming, &len);
      if ( p < 0 ) _perror("accept", p); 

      struct hostent* ihp = gethostbyaddr(&incoming.sin_addr, sizeof(struct in_addr), AF_INET);
      printf(">> Connected to %s\n", ihp->h_name);
 
      /* read and print strings sent over the connection */
      while ( 1 ) {
          if ( !rdMsg(p, buf, 32, 0) ) break;
          if ( !strcmp("close", buf) ) break;

          printf("MSG > %s\n", buf);

          Potato p = Potato_take(buf); 
          Potato_print(p);
          Potato_free(p); 
      }
      close(p);
      printf(">> Connection closed\n");
    }
    close(s);
    exit(0);
}

