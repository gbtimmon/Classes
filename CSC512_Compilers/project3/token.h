#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>

// Token.h.gen 
// contains a generated enum of the available tokens.
// plus a helper array that contains string names for tokens,
// string names are a bit of a hack but only needed for debugging. 
#include "token.gen.h"

typedef struct _token { 
    token_t type;             // what kind of token am I
    char*   value;            // what is the char data from the original source (If Im terminal )
    int    id;                // A unique ID to keep the tree eaiser to debug. 
    void * data;              // A struct contiaining special data used during transform. 
    struct _token * parent;   // The symbol that created me. 
    struct _token * child;    // The head of a doubly linked lists of the symbols I created. 
    struct _token * peer;     // The next in the dobuly linked list
    struct _token * lpeer;    // The prev in the doubley linked list
} * Token;

typedef struct _tokenStackElement {
    struct _token * token;
    struct _tokenStackElement * prev;
} * TokenStackElement;

typedef struct _token_stack {
    struct _tokenStackElement * head;
} * TokenStack;

int        isTerminal     ( token_t );
// Report if a token_t is a terminal type. Its hacky, but it works and is quick to code. 
//     @1 : token type to check if terminal

int        isSkip         ( token_t );
// Report if the parser should skip this token.
//     @1 : token that is being checked.

void       Token_appendChild ( Token, Token );
// Add a child to the current token at the end of the list
//     @1 : the token to add 

void       Token_prependChild( Token, Token ); 
// Add a child to the current token at the start of the list. 
//     @1 : the token to prepend

void       Token_printTree( Token );
// Output the IR tree
//     @1 : The head of the tree

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

char *     Token_toString( Token ); 
// Returns a string on the head that is a representation of the pass in token. 
//     @1 : Token to represent

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
