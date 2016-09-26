#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>

// token types for a LL(1) Parser. 
typedef enum _token_t {
/*ops    */ T_ADD = 0, //0
            T_MULT,
            T_RPAR,
            T_LPAR,
            T_RBRAC,
            T_LBRAC,
            T_COMMA,
            T_SEMI,
            T_OP, // COME BACK TO THIS TO FIX.
       
/*skips  */ T_COMMENT,          
            T_META,
            T_MCOMMENT,

/*ident  */ T_VAR,
            T_TYPE,
            T_KEYWORD, // COME BACK TO THIS TO FIX.
            T_IDENT, // COME BACK TO THIS TO FIX.

/*number */ T_NUMBER,
/*string */ T_STRING,
/*eof    */ T_EOF,              
            T_e,
            S_DATA,
            S_IDLIST,
            S_IDLIST_,
            S_ID,
            S_ID_,
            S_EXP,
            S_EXP_,
            S_TERM,
            S_TERM_,
            S_FACT,
            S_FACT_
} token_t;

#define TOKEN_COUNT  (S_FACT_ + 1)
#define TERM_COUNT   (T_EOF + 1)
#define SYM_COUNT    (TOKEN_COUNT - TERM_COUNT)

typedef struct _token { 
    token_t type;
    char*   value;
    struct _token * prev;
} * Token;

typedef struct _token_stack {
    struct _token * head
} * TokenStack;

int        isTerminal     ( token_t );
// Report if a token_t is a terminal type. Its hacky, but it works and is quick to code. 
//     @1 : token type to check if terminal

int        isSkip         ( token_t );
// Report if the parser should skip this token.
//     @1 : token that is being checked.

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
