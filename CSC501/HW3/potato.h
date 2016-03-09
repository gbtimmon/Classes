
#ifndef POTATO_H
#define POTATO_H

typedef struct signature_t {
    struct signature_t *next; 
    int id; 
    //char* hostname; 
} *Signature;

typedef struct potato_t {
    int initial_count; 
    int current_count; 
    Signature signature; 
} *Potato;

typedef int Stream; 
typedef char* String; 

Potato Potato_cook(int);
Potato Potato_sign(Potato, int);
Potato Potato_give(Potato, Stream);
Potato Potato_take(Stream); 
Potato Potato_free(Potato); 
#endif
