#ifndef SOCKET_C
#define SOCKET_C

#define _GNU_SOURCE
#define BUFFER_SIZE 512

#include "connection.h"
#include "socket.h"

#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int SocketListener_new( Connection c ) { 
   
    char hostname[BUFFER_SIZE];
    gethostname(hostname, sizeof hostname);
 
    struct hostent *hp = gethostbyname(hostname);
    if( hp == NULL ) {
        fprintf(stderr, "SocketListenr_new: host not found (%s)\n", hostname);
        return -1; 
    }

    int s = socket( AF_INET, SOCK_STREAM, 0);   
    if( s < 0 ) {
        perror("socket:");
        return -1; 
    }
     
    struct sockaddr_in sin; 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(c->port);
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

#define TIMEOUT 30
int SocketWriter_new( Connection c ){

    struct hostent *hp = gethostbyname( c->host );
    if ( hp == NULL ) {
        fprintf( stderr, "SocektWriter_new: host not found (%s)\n", c->host );
        return -1; 
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if ( s < 0 ) {
        perror("socket:");
        return -1;  
    }

    struct sockaddr_in sin; 
    sin.sin_family = AF_INET;
    sin.sin_port = htons( c->port );
    memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);

    int rc = -1; 
    int secs = 0;

    while ( 1 ) { 

        int rc = connect(s, (struct sockaddr *)&sin, sizeof(struct sockaddr));

        if( rc >= 0) 
           break;

        if( ++secs > TIMEOUT ) {
            fprintf(stderr, "connection timed out, could not connect to server\n"); 
            exit(1); 
        }
       
        printf("%d\n", secs);
         
        sleep(1); 
    }

    return s; 

}

void Socket_sendi( int s, int v){

    char t[BUFFER_SIZE];

    sprintf(t, "%d\n\0", v);
    printf("SEND> %s\n", t); 

    int l = send( s, t, strlen(t), 0);

    if( l != strlen(t) ) {
        perror("send");
        exit(1);
    }

}

void Socket_sends( int s, char* i ){
    return;
}

char* Socket_recv( int c ) {
    char* s = malloc( sizeof(char) * 1024 ); 
    s[0] = '\0';
    char t[1024]; 

    while( 1 ) {
        int len = recv(c, t, 1024, 0); 
        if( len == 0 ) break;
        if( len <  0 ) {
            perror("recv");
            return len;
        }
        t[len] = '\0';
        strcat(s, t);    
    }
    return s; 
}

char* Socket_get_message( int s ){
    struct sockaddr_in i;
    unsigned int l = sizeof(struct sockaddr);
    int p = accept(s, (struct sockaddr *) &i, &l);
    if ( p < 0 ){
        perror("accept"); 
        exit(p);
    }
    struct hostent* ihp = gethostbyaddr(&i.sin_addr, sizeof(struct in_addr), AF_INET);
    printf(">> Connected to %s\n", ihp->h_name);

    char * ret = Socket_recv( p ); 
    close(p); 
    return ret; 
}

#endif

