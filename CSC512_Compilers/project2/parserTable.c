#ifndef _PARSER_TABLE_C_
#define _PARSER_TABLE_C_

#include "token.h"

#define RULE_ERROR -1 
#define TERMINAL_COUNT

token_t rule[][5] = {
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



int ** getParserTable ( ) {
    int ** arr = malloc( sizeof( token_t * ) * SYM_COUNT );
    for( int i = 0; i < SYM_COUNT; i++ ) {
        arr[i] = malloc( sizeof( token_t ) * TERM_COUNT );
        for( int j = 0; j < TERM_COUNT; j++ ) {
           arr[i][j] = RULE_ERROR;
        }
        
    }
    arr[ S_DATA    ][ T_TYPE   ] = 1;
    arr[ S_DATA    ][ T_EOF    ] = 2;
    arr[ S_IDLIST  ][ T_VAR    ] = 3;
    arr[ S_IDLIST_ ][ T_COMMA  ] = 4;
    arr[ S_IDLIST_ ][ T_SEMI   ] = 5;
    arr[ S_ID      ][ T_VAR    ] = 6;
    arr[ S_ID_     ][ T_LBRAC  ] = 7;
    arr[ S_ID_     ][ T_COMMA  ] = 8;
    arr[ S_ID_     ][ T_SEMI   ] = 8;
    arr[ S_EXP     ][ T_VAR    ] = 9;
    arr[ S_EXP     ][ T_NUMBER ] = 9;
    arr[ S_EXP     ][ T_LPAR   ] = 9;
    arr[ S_EXP_    ][ T_ADD    ] = 10;
    arr[ S_EXP_    ][ T_RBRAC  ] = 11;
    arr[ S_EXP_    ][ T_RPAR   ] = 11; 
    arr[ S_TERM    ][ T_VAR    ] = 12;
    arr[ S_TERM    ][ T_NUMBER ] = 12;
    arr[ S_TERM    ][ T_LPAR   ] = 12;
    arr[ S_TERM_   ][ T_MULT   ] = 13;
    arr[ S_TERM_   ][ T_ADD    ] = 14;
    arr[ S_TERM_   ][ T_RPAR   ] = 14;
    arr[ S_TERM_   ][ T_RBRAC  ] = 14;
    arr[ S_FACT    ][ T_VAR    ] = 15;
    arr[ S_FACT    ][ T_NUMBER ] = 16;
    arr[ S_FACT    ][ T_LPAR   ] = 17;
    arr[ S_FACT_   ][ T_MULT   ] = 19;
    arr[ S_FACT_   ][ T_ADD    ] = 19;
    arr[ S_FACT_   ][ T_RBRAC  ] = 19;
    arr[ S_FACT_   ][ T_RPAR   ] = 19;

    return arr;
};


#endif
