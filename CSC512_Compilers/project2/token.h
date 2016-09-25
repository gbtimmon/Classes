#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>
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

Token      Token_new      ( token_t, char * ); 
Token      Token_free     ();
void       Token_write    ( Token, FILE * );
TokenStack TokenStack_new ( void );
void       TokenStack_free( TokenStack s ); 
void       TokenStack_push( TokenStack s, Token t ); 
Token      TokenStack_pop ( TokenStack s );

#endif
