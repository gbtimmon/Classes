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
    // generated table lookups
    #include <parserTable.gen.c>
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
