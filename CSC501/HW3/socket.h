#ifndef SOCKET_H
#define SOCKET_H

#include "connection.h"

#define MSG_TYPE_CLOSE      1
#define MSG_TYPE_POTATO     2
#define MSG_TYPE_CONNECTION 3

#define MSG_DEST_MASTER     1
#define MSG_DEST_LEFT       2
#define MSG_DEST_RIGHT      3

void  Socket_sendi(int, int); 
void  Socket_sends(int, char*); 
char* Socket_recv(int); 
int   SocketListener_new( Connection );
int   SocketWriter_new( Connection ); 
#endif

