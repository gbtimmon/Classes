#ifndef CONNECTION_H
#define CONNECTION_H

#define  CONN_TYPE_CHILD  0
#define  CONN_TYPE_IN     1
#define  CONN_TYPE_LEFT   2
#define  CONN_TYPE_RIGHT  3
#define  CONN_TYPE_MASTER 4

typedef struct connection {
    int type; 
    char *host;
    int  port; 
    int num;
} * Connection; 

Connection Connection_new( const char *, int, int, int); 
void       Connection_free( Connection );
void       Connection_send( Connection, int ); 
Connection Connection_recv( char ** ); 
void       Connection_print( Connection ); 

#endif
