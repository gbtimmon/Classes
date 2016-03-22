#ifndef CONNECTION_H
#define CONNECTION_H

typedef struct connection {
    int  type; 
    char *host;
    int  port; 
} * Connection; 

Connection Connection_new( const char *, int); 
void       Connection_free( Connection );

#endif
