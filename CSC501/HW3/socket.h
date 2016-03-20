#ifndef SOCKET_H
#define SOCKET_H

#define MSG_TYPE_POTATO       1
#define MSG_TYPE_MASTER       2
#define MSG_TYPE_LEFT_SOCKET  3
#define MSG_TYPE_RIGHT_SOCKET 4

void Socket_send(int, int); 
int  Socket_recv(int); 
int  SocketListener_new(int);
int  SocketWriter_new(const char *, int); 
#endif

