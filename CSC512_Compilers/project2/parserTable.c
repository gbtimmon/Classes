#ifndef _PARSER_TABLE_C_
#define _PARSER_TABLE_C_

#include "token.h"
#include "parserTable.h"

#define RULE_ERROR -1 
#define TERMINAL_COUNT

int ** lookupTable;
void initLookupTable()
{
    lookupTable = malloc( sizeof( token_t * ) * SYM_COUNT );
    
    for( int i = 0; i < SYM_COUNT; i++ ) 
    {
        lookupTable[i] = malloc( sizeof( token_t ) * TERM_COUNT );

        for( int j = 0; j < TERM_COUNT; j++ ) 
        {
           lookupTable[i][j] = RULE_ERROR;
        }
    }

    set( S_DATA    , T_TYPE   , 1);
    set( S_DATA    , T_EOF    , 2);
    set( S_IDLIST  , T_VAR    , 3);
    set( S_IDLIST_ , T_COMMA  , 4);
    set( S_IDLIST_ , T_SEMI   , 5);
    set( S_ID      , T_VAR    , 6);
    set( S_ID_     , T_LBRAC  , 7);
    set( S_ID_     , T_COMMA  , 8);
    set( S_ID_     , T_SEMI   , 8);
    set( S_EXP     , T_VAR    , 9);
    set( S_EXP     , T_NUMBER , 9);
    set( S_EXP     , T_LPAR   , 9);
    set( S_EXP_    , T_ADD    ,10);
    set( S_EXP_    , T_RBRAC  ,11);
    set( S_EXP_    , T_RPAR   ,11); 
    set( S_TERM    , T_VAR    ,12);
    set( S_TERM    , T_NUMBER ,12);
    set( S_TERM    , T_LPAR   ,12);
    set( S_TERM_   , T_MULT   ,13);
    set( S_TERM_   , T_ADD    ,14);
    set( S_TERM_   , T_RPAR   ,14);
    set( S_TERM_   , T_RBRAC  ,14);
    set( S_FACT    , T_VAR    ,15);
    set( S_FACT    , T_NUMBER ,16);
    set( S_FACT    , T_LPAR   ,17);
    set( S_FACT_   , T_MULT   ,19);
    set( S_FACT_   , T_ADD    ,19);
    set( S_FACT_   , T_RBRAC  ,19);
    set( S_FACT_   , T_RPAR   ,19);
};

void set( token_t stack, token_t term, int rule )
{
    lookupTable[ stack - TERM_COUNT ][ term ] = rule;
};


int lookup( token_t stack, token_t term )
{
    return lookupTable[ stack - TERM_COUNT ][ term ] - 1;
};

#endif
