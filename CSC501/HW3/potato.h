
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

Potato Potato_cook(int);
Potato Potato_give(Potato, int, Stream);
Potato Potato_take(String); 
Potato Potato_print(Potato, Stream); 
void   Potato_free(Potato); 
#endif
