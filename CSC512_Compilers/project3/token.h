#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>

// Token.h.gen 
// contains a generated enum of the available tokens.
// plus a helper array that contains string names for tokens,
// string names are a bit of a hack but only needed for debugging. 

// EDITS FOR PROJECT 3 
// So I can build a tree from the symbols as the comthrough the parser, 
// and produce a IR for the code, I have added an additional pointers "sibling" 
// and "child". Ive rewritten the stack code to keep the token code uncluttered. 
// and pop will clear existing pointers. As we pop and create each 
#include "token.gen.h"

typedef struct _token { 
    token_t type;
    char*   value;
    struct _token * peer;
    struct _token * child;
} * Token;

typedef struct _token_stack {
    struct _token_stack_ele * head;
} * TokenStack;

typedef struct _token_stack_ele {
    struct _token * ele;
    struct _token_stack_ele * next;
} * TokenStackEle; 

int        isTerminal     ( token_t );
// Report if a token_t is a terminal type. Its hacky, but it works and is quick to code. 
//     @1 : token type to check if terminal

int        isSkip         ( token_t );
// Report if the parser should skip this token.
//     @1 : token that is being checked.

Token      Token_new      ( token_t, char * );
// Create a new token struct. 
//     @1 : token type
//     @2 : token string data, only filled on terminal 

void       Token_free     ( Token t );
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

void TokenStack_write( TokenStack, FILE * );

#endif
