#ifndef SOCKET_H
#define SOCKET_H

#include "connection.h"

#define MSG_TYPE_CLOSE 1
#define MSG_TYPE_POTATO 2
#define MSG_TYPE_CHILD_NAME 3
#define MSG_TYPE_CONNECTION 4


void  Socket_sendi(int, int); 
void  Socket_sends(int, char*); 
int   SocketListener_new( Connection );
int   SocketWriter_new( Connection ); 
char* Socket_get_message( int );
#endif

