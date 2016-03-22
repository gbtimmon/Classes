#ifndef CONNECTION_H
#define CONNECTION_H

#define CONN_TYPE_IN     0
#define CONN_TYPE_LEFT   1
#define CONN_TYPE_RIGHT  2
#define CONN_TYPE_MASTER 3
typedef struct connection {
    int  type; 
    char *host;
    int  port; 
} * Connection; 

Connection Connection_new( const char *, int, int); 
void       Connection_free( Connection );
void       Connection_send( Connection, int ); 
Connection Connection_recv( char ** ); 

#endif
