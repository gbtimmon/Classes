#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>

// token types for a LL(1) Parser. 
typedef enum _token_t {
    T_ADD,
    T_CMP,
    T_COMMENT,
    T_COP,
    T_DATA,
    T_EMPTY,
    T_EOF,
    T_EXP,
    T_EXP_PRIME,
    T_FAC,
    T_ID,
    T_ID_PRIME,
    T_IDENT,
    T_IDLIST,
    T_IDLIST_PRIME,
    T_KEYWORD,
    T_MCOMMENT,
    T_META,
    T_MULT,
    T_NUMBER,
    T_OP,
    T_STRING,
    T_TERM,
    T_TERM_PRIME,
    T_TYPE,
    T_VAR
} token_t;

typedef struct _token { 
    token_t type;
    char*   value;
    struct _token * prev;
} * Token;

typedef struct _token_stack {
    struct _token * head
} * TokenStack;

Token      Token_new      ( token_t, char *, int );
// Create a new token struct. 
//     @1 : token type
//     @2 : token string data, only filled on terminal 
//     @3 : terminal indicator, true ( != 0 ) or false ( == 0 )

Token      Token_free     ( Token t );
// Free a token, properly releasing all internal memory
//     @1 : The token to free

void       Token_write    ( Token, FILE * );
// Write a token to a file object. 
//     @1 : The token to write

TokenStack TokenStack_new ( void );
// Create a token stack. Overkill for such a simple structure but I like the consitent
// coding style since it behaves like the other objects in this project. 

void       TokenStack_free( TokenStack s ); 
// Frees a token stack, has a side effect of freeing all tokens inside the stack as well
//     @1 : the tokenstack to free. 

void       TokenStack_push( TokenStack s, Token t ); 
// Pushs a token onto a stack
//     @1 : The stack to push too 
//     @2 : The token to push

Token      TokenStack_pop ( TokenStack s );
// Pops a token off a stack
//     @1 : The stack to pop from. 

#endif
