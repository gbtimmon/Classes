//Rules table
token_t ruleTable [][5]= {
/*  1 : S_TYPE        */ { T_VOID       , T_e          , T_e          , T_e          , T_e          , },
/*  2 : S_TYPE        */ { T_TYPE       , T_e          , T_e          , T_e          , T_e          , },
/*  3 : S_ADD         */ { T_PLUS       , T_e          , T_e          , T_e          , T_e          , },
/*  4 : S_ADD         */ { T_MINUS      , T_e          , T_e          , T_e          , T_e          , },
/*  5 : S_START       */ { S_TYPE       , T_VAR        , S_START_A    , T_e          , T_e          , },
/*  6 : S_START       */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/*  7 : S_START_A     */ { S_BRACK_EXP  , S_IDLIST     , T_SEMI       , S_START      , T_e          , },
/*  8 : S_START_A     */ { T_LPAR       , S_PARM_LIST  , T_RPAR       , S_FUNC_DEF   , S_CODE       , },
/*  9 : S_BRACK_EXP   */ { T_LBRACK     , S_EXP        , T_RBRACK     , T_e          , T_e          , },
/* 10 : S_BRACK_EXP   */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 11 : S_IDLIST      */ { T_COMMA      , T_VAR        , S_BRACK_EXP  , S_IDLIST     , T_e          , },
/* 12 : S_IDLIST      */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 13 : S_PARM_LIST   */ { T_TYPE       , T_VAR        , S_PARM_LIST_A, T_e          , T_e          , },
/* 14 : S_PARM_LIST   */ { T_VOID       , S_PARM_LIST_C, T_e          , T_e          , T_e          , },
/* 15 : S_PARM_LIST   */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 16 : S_PARM_LIST_A */ { T_COMMA      , S_PARM_LIST_B, T_e          , T_e          , T_e          , },
/* 17 : S_PARM_LIST_A */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 18 : S_PARM_LIST_B */ { S_TYPE       , T_VAR        , S_PARM_LIST_A, T_e          , T_e          , },
/* 19 : S_PARM_LIST_C */ { T_VAR        , S_PARM_LIST_A, T_e          , T_e          , T_e          , },
/* 20 : S_PARM_LIST_C */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 21 : S_FUNC        */ { S_TYPE       , T_VAR        , T_LPAR       , S_PARM_LIST  , T_RPAR       , },
/* 22 : S_FUNC_DEF    */ { T_LCURL      , S_DATA       , S_STATEMENT  , T_RCURL      , T_e          , },
/* 23 : S_FUNC_DEF    */ { T_SEMI       , T_e          , T_e          , T_e          , T_e          , },
/* 24 : S_DATA        */ { S_TYPE       , T_VAR        , S_IDLIST     , T_SEMI       , S_DATA       , },
/* 25 : S_DATA        */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 26 : S_STATEMENT   */ { T_VAR        , S_STATEMENT_A, S_STATEMENT  , T_e          , T_e          , },
/* 27 : S_STATEMENT_A */ { S_BRACK_EXP  , T_EQ         , S_EXP        , T_SEMI       , T_e          , },
/* 28 : S_STATEMENT_A */ { T_LPAR       , S_EXP_LIST   , T_RPAR       , T_SEMI       , T_e          , },
/* 29 : S_STATEMENT   */ { T_WHILE      , S_STATEMENT_B, S_STATEMENT  , T_e          , T_e          , },
/* 30 : S_STATEMENT   */ { T_IF         , S_STATEMENT_B, S_STATEMENT  , T_e          , T_e          , },
/* 31 : S_STATEMENT_B */ { T_LPAR       , S_COND_EXP   , T_RPAR       , S_BLOCK      , T_e          , },
/* 32 : S_STATEMENT   */ { T_RETURN     , S_STATEMENT_R, T_SEMI       , S_STATEMENT  , T_e          , },
/* 33 : S_STATEMENT_R */ { S_EXP        , T_e          , T_e          , T_e          , T_e          , },
/* 34 : S_STATEMENT_R */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 35 : S_STATEMENT   */ { T_READ       , S_STATEMENT_C, T_SEMI       , S_STATEMENT  , T_e          , },
/* 36 : S_STATEMENT_C */ { T_LPAR       , T_VAR        , S_BRACK_EXP  , T_RPAR       , T_e          , },
/* 37 : S_STATEMENT   */ { T_BREAK      , T_SEMI       , S_STATEMENT  , T_e          , T_e          , },
/* 38 : S_STATEMENT   */ { T_CONTINUE   , T_SEMI       , S_STATEMENT  , T_e          , T_e          , },
/* 39 : S_STATEMENT   */ { T_WRITE      , S_STATEMENT_D, T_SEMI       , S_STATEMENT  , T_e          , },
/* 40 : S_STATEMENT_D */ { T_LPAR       , S_EXP        , T_RPAR       , T_e          , T_e          , },
/* 41 : S_STATEMENT   */ { T_PRINT      , S_STATEMENT_P, T_SEMI       , S_STATEMENT  , T_e          , },
/* 42 : S_STATEMENT_P */ { T_LPAR       , T_STRING     , T_RPAR       , T_e          , T_e          , },
/* 43 : S_STATEMENT   */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 44 : S_CODE        */ { S_FUNC       , S_FUNC_DEF   , S_CODE       , T_e          , T_e          , },
/* 45 : S_CODE        */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 46 : S_EXP         */ { S_TERM       , S_EXP_A      , T_e          , T_e          , T_e          , },
/* 47 : S_EXP_A       */ { S_ADD        , S_TERM       , S_EXP_A      , T_e          , T_e          , },
/* 48 : S_EXP_A       */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 49 : S_TERM        */ { S_FACT       , S_TERM_A     , T_e          , T_e          , T_e          , },
/* 50 : S_TERM_A      */ { T_MULT       , S_FACT       , S_TERM_A     , T_e          , T_e          , },
/* 51 : S_TERM_A      */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 52 : S_FACT        */ { T_VAR        , S_FACT_A     , T_e          , T_e          , T_e          , },
/* 53 : S_FACT_A      */ { S_BRACK_EXP  , T_e          , T_e          , T_e          , T_e          , },
/* 54 : S_FACT_A      */ { T_LPAR       , S_EXP_LIST   , T_RPAR       , T_e          , T_e          , },
/* 55 : S_FACT_A      */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 56 : S_FACT        */ { T_NUMBER     , T_e          , T_e          , T_e          , T_e          , },
/* 57 : S_FACT        */ { T_MINUS      , T_NUMBER     , T_e          , T_e          , T_e          , },
/* 58 : S_FACT        */ { T_LPAR       , S_EXP        , T_RPAR       , T_e          , T_e          , },
/* 59 : S_COND_EXP    */ { S_COND       , S_COND_EXP_A , T_e          , T_e          , T_e          , },
/* 60 : S_COND_EXP_A  */ { T_BOOL_OP    , S_COND       , T_e          , T_e          , T_e          , },
/* 61 : S_COND_EXP_A  */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 62 : S_COND        */ { S_EXP        , T_CMP        , S_EXP        , T_e          , T_e          , },
/* 63 : S_BLOCK       */ { T_LCURL      , S_STATEMENT  , T_RCURL      , T_e          , T_e          , },
/* 64 : S_EXP_LIST    */ { S_EXP        , S_EXP_LIST_A , T_e          , T_e          , T_e          , },
/* 65 : S_EXP_LIST    */ { T_STRING     , S_EXP_LIST_A , T_e          , T_e          , T_e          , },
/* 66 : S_EXP_LIST    */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 67 : S_EXP_LIST_A  */ { T_COMMA      , S_EXP_LIST_B , T_e          , T_e          , T_e          , },
/* 68 : S_EXP_LIST_A  */ { T_e          , T_e          , T_e          , T_e          , T_e          , },
/* 69 : S_EXP_LIST_B  */ { S_EXP        , S_EXP_LIST_A , T_e          , T_e          , T_e          , },
/* 70 : S_EXP_LIST_B  */ { T_STRING     , S_EXP_LIST_A , T_e          , T_e          , T_e          , },
};
