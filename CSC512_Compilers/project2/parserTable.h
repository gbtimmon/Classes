#ifndef _PARSER_TABLE_C_
#define _PARSER_TABLE_C_

#include "token.h"

#define RULE_ERROR -1 
#define RULE_LENGTH 5

#include "parserTable.gen.h"

int ** lookupTable; 
void initLookupTable ( );
int lookup( token_t, token_t );


#endif
