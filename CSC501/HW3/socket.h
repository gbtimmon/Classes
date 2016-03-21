#ifndef SOCKET_H
#define SOCKET_H

#include "connection.h"

#define MSG_TYPE_POTATO       1
#define MSG_TYPE_MASTER       2
#define MSG_TYPE_LEFT_SOCKET  3
#define MSG_TYPE_RIGHT_SOCKET 4

void  Socket_send(int, int); 
char* Socket_recv(int); 
int   SocketListener_new( Connection );
int   SocketWriter_new( Connection ); 
#endif

