//lookups 
set( S_TYPE       , T_VOID       , 1 );
set( S_TYPE       , T_TYPE       , 2 );
set( S_ADD        , T_PLUS       , 3 );
set( S_ADD        , T_MINUS      , 4 );
set( S_START      , T_TYPE       , 5 );
set( S_START      , T_VOID       , 5 );
set( S_START      , T_EOF        , 6 );
set( S_START_A    , T_LBRACK     , 7 );
set( S_START_A    , T_COMMA      , 7 );
set( S_START_A    , T_SEMI       , 7 );
set( S_START_A    , T_LPAR       , 8 );
set( S_BRACK_EXP  , T_EQ         , 9 );
set( S_BRACK_EXP  , T_LBRACK     , 9 );
set( S_BRACK_EXP  , T_COMMA      , 10 );
set( S_BRACK_EXP  , T_SEMI       , 10 );
set( S_BRACK_EXP  , T_RPAR       , 10 );
set( S_BRACK_EXP  , T_BOOL_OP    , 10 );
set( S_BRACK_EXP  , T_EQ         , 10 );
set( S_BRACK_EXP  , T_LPAR       , 10 );
set( S_BRACK_EXP  , T_RPAR       , 10 );
set( S_IDLIST     , T_COMMA      , 11 );
set( S_IDLIST     , T_SEMI       , 12 );
set( S_PARM_LIST  , T_TYPE       , 13 );
set( S_PARM_LIST  , T_VOID       , 14 );
set( S_PARM_LIST  , T_RPAR       , 15 );
set( S_PARM_LIST_A, T_COMMA      , 16 );
set( S_PARM_LIST_A, T_RPAR       , 17 );
set( S_PARM_LIST_B, T_VOID       , 18 );
set( S_PARM_LIST_B, T_TYPE       , 18 );
set( S_PARM_LIST_C, T_VAR        , 19 );
set( S_PARM_LIST_C, T_RPAR       , 20 );
set( S_FUNC       , T_VOID       , 21 );
set( S_FUNC       , T_TYPE       , 21 );
set( S_FUNC_DEF   , T_LCURL      , 22 );
set( S_FUNC_DEF   , T_SEMI       , 23 );
set( S_DATA       , T_VOID       , 24 );
set( S_DATA       , T_TYPE       , 24 );
set( S_DATA       , T_VAR        , 25 );
set( S_DATA       , T_RCURL      , 25 );
set( S_DATA       , T_WHILE      , 25 );
set( S_DATA       , T_IF         , 25 );
set( S_DATA       , T_RETURN     , 25 );
set( S_DATA       , T_BREAK      , 25 );
set( S_DATA       , T_CONTINUE   , 25 );
set( S_DATA       , T_READ       , 25 );
set( S_DATA       , T_WRITE      , 25 );
set( S_DATA       , T_PRINT      , 25 );
set( S_STATEMENT  , T_VAR        , 26 );
set( S_STATEMENT_A, T_LBRACK     , 27 );
set( S_STATEMENT_A, T_EQ         , 27 );
set( S_STATEMENT_A, T_LPAR       , 28 );
set( S_STATEMENT  , T_WHILE      , 29 );
set( S_STATEMENT  , T_IF         , 30 );
set( S_STATEMENT_B, T_LPAR       , 31 );
set( S_STATEMENT  , T_RETURN     , 32 );
set( S_STATEMENT_R, T_VAR        , 33 );
set( S_STATEMENT_R, T_NUMBER     , 33 );
set( S_STATEMENT_R, T_MINUS      , 33 );
set( S_STATEMENT_R, T_LPAR       , 33 );
set( S_STATEMENT_R, T_SEMI       , 34 );
set( S_STATEMENT  , T_READ       , 35 );
set( S_STATEMENT_C, T_LPAR       , 36 );
set( S_STATEMENT  , T_BREAK      , 37 );
set( S_STATEMENT  , T_CONTINUE   , 38 );
set( S_STATEMENT  , T_WRITE      , 39 );
set( S_STATEMENT_D, T_LPAR       , 40 );
set( S_STATEMENT  , T_PRINT      , 41 );
set( S_STATEMENT_P, T_LPAR       , 42 );
set( S_STATEMENT  , T_RCURL      , 43 );
set( S_CODE       , T_VOID       , 44 );
set( S_CODE       , T_TYPE       , 44 );
set( S_CODE       , T_EOF        , 45 );
set( S_EXP        , T_VAR        , 46 );
set( S_EXP        , T_NUMBER     , 46 );
set( S_EXP        , T_MINUS      , 46 );
set( S_EXP        , T_LPAR       , 46 );
set( S_EXP_A      , T_PLUS       , 47 );
set( S_EXP_A      , T_MINUS      , 47 );
set( S_EXP_A      , T_RPAR       , 48 );
set( S_EXP_A      , T_SEMI       , 48 );
set( S_EXP_A      , T_RBRACK     , 48 );
set( S_EXP_A      , T_CMP        , 48 );
set( S_EXP_A      , T_BOOL_OP    , 48 );
set( S_TERM       , T_VAR        , 49 );
set( S_TERM       , T_LPAR       , 49 );
set( S_TERM       , T_NUMBER     , 49 );
set( S_TERM       , T_MINUS      , 49 );
set( S_TERM_A     , T_MULT       , 50 );
set( S_TERM_A     , T_PLUS       , 51 );
set( S_TERM_A     , T_MINUS      , 51 );
set( S_TERM_A     , T_CMP        , 51 );
set( S_TERM_A     , T_RPAR       , 51 );
set( S_TERM_A     , T_BOOL_OP    , 51 );
set( S_TERM_A     , T_SEMI       , 51 );
set( S_FACT       , T_VAR        , 52 );
set( S_FACT_A     , T_LPAR       , 53 );
set( S_FACT_A     , T_RPAR       , 54 );
set( S_FACT_A     , T_BOOL_OP    , 54 );
set( S_FACT_A     , T_SEMI       , 54 );
set( S_FACT       , T_NUMBER     , 55 );
set( S_FACT       , T_MINUS      , 56 );
set( S_FACT       , T_LPAR       , 57 );
set( S_COND_EXP   , T_NUMBER     , 58 );
set( S_COND_EXP   , T_VAR        , 58 );
set( S_COND_EXP   , T_LPAR       , 58 );
set( S_COND_EXP   , T_MINUS      , 58 );
set( S_COND_EXP_A , T_BOOL_OP    , 59 );
set( S_COND_EXP_A , T_RPAR       , 60 );
set( S_COND       , T_NUMBER     , 61 );
set( S_COND       , T_VAR        , 61 );
set( S_COND       , T_LPAR       , 61 );
set( S_COND       , T_MINUS      , 61 );
set( S_BLOCK      , T_LCURL      , 62 );
set( S_EXP_LIST   , T_VAR        , 63 );
set( S_EXP_LIST   , T_NUMBER     , 63 );
set( S_EXP_LIST   , T_MINUS      , 63 );
set( S_EXP_LIST   , T_LPAR       , 63 );
set( S_EXP_LIST   , T_STRING     , 64 );
set( S_EXP_LIST   , T_RPAR       , 65 );
set( S_EXP_LIST_A , T_COMMA      , 66 );
set( S_EXP_LIST_A , T_RPAR       , 67 );
set( S_EXP_LIST_B , T_VAR        , 68 );
set( S_EXP_LIST_B , T_NUMBER     , 68 );
set( S_EXP_LIST_B , T_MINUS      , 68 );
set( S_EXP_LIST_B , T_LPAR       , 68 );
set( S_EXP_LIST_B , T_STRING     , 69 );
