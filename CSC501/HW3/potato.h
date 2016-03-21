
#ifndef POTATO_H
#define POTATO_H

typedef struct potato_t {
    int  initial_count; 
    int  current_count; 
    int  signature_count; 
    int* signature; 
} *Potato;

typedef int Stream; 
typedef char* String; 

Potato Potato_new(int);
void   Potato_send(Potato p,int i, int socket);
Potato Potato_recv(char**); 
Potato Potato_print(Potato); 
Potato Potato_fprint(Potato, Stream); 
void   Potato_free(Potato); 
#endif
