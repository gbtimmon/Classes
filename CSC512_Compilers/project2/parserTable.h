#ifndef _PARSER_TABLE_C_
#define _PARSER_TABLE_C_

#include "token.h"

#define RULE_ERROR -1 
#define TERMINAL_COUNT

token_t ruleTable[][5] = {
/* 1*/  { T_TYPE,   S_IDLIST,  T_SEMI,    S_DATA, T_e },
/* 2*/  { T_e,      T_e,       T_e,       T_e,    T_e },
/* 3*/  { S_ID,     S_IDLIST_, T_e,       T_e,    T_e },
/* 4*/  { T_COMMA,  S_ID,      S_IDLIST_, T_e,    T_e },
/* 5*/  { T_e,      T_e,       T_e,       T_e,    T_e },
/* 6*/  { T_VAR,    S_ID_,     T_e,       T_e,    T_e },
/* 7*/  { T_LBRAC,  S_EXP,     T_RBRAC,   T_e,    T_e },
/* 8*/  { T_e,      T_e,       T_e,       T_e,    T_e },
/* 9*/  { S_TERM,   S_EXP_,    T_e,       T_e,    T_e },
/*10*/  { T_ADD,    S_TERM,    S_EXP_,    T_e,    T_e },
/*11*/  { T_e,      T_e,       T_e,       T_e,    T_e },
/*12*/  { S_FACT,   S_TERM_,   T_e,       T_e,    T_e },
/*13*/  { T_MULT,   S_FACT,    S_TERM_,   T_e,    T_e },
/*14*/  { T_e,      T_e,       T_e,       T_e,    T_e },
/*15*/  { T_VAR,    S_FACT_,   T_e,       T_e,    T_e },
/*16*/  { T_NUMBER, T_e,       T_e,       T_e,    T_e },
/*17*/  { T_LPAR,   S_EXP,     T_RPAR,    T_e,    T_e },
/*18*/  { T_LBRAC,  S_EXP,     T_RBRAC,   T_e,    T_e },
/*19*/  { T_e,      T_e,       T_e,       T_e,    T_e }
};

int ** lookupTable; 
void initLookupTable ( );
int lookup( token_t, token_t );


#endif
