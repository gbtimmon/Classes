#ifndef SOCKET_C
#define SOCKET_C

#define _GNU_SOURCE

#include "socket.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int SocketListener_new(int port) { 
   
    char hostname[256];
    gethostname(hostname, sizeof hostname);
 
    struct hostent *hp = gethostbyname(hostname);
    if( hp == NULL ) {
        fprintf(stderr, "host not found (%s)\n", hostname);
        return -1; 
    }

    int s = socket( AF_INET, SOCK_STREAM, 0);   
    if( s < 0 ) {
        perror("socket:");
        return -1; 
    }
     
    struct sockaddr_in sin; 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);

    int rc = bind(s, &sin, sizeof( struct sockaddr_in ));
    if( rc < 0 ) {
        perror("bind:");
        return -1; 
    }
        
    rc =  listen( s, 5);
    if( rc < 0 ) {
        perror("listen:");
        return -1; 
    }
    return s; 
}

#endif

