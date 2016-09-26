S_TYPE        :: T_VOID                                                          FIRST+: T_VOID 
S_TYPE        :: T_TYPE                                                          FIRST+: T_TYPE
S_ADD         :: T_ADD                                                           FIRST+: T_ADD
S_ADD         :: T_MINUS                                                         FIRST+: T_MINUS
S_START       :: S_TYPE       T_VAR         S_START_A                            FIRST+: T_TYPE   T_VOID
S_START       :: T_e                                                             FIRST+: T_EOF
S_START_A     :: S_BRACK_EXP  S_IDLIST      T_SEMI        S_START                FIRST+: T_LBRACK T_COMMA T_SEMI
S_START_A     :: T_LPAR       S_PARM_LIST   T_RPAR        S_FUNC_DEF    S_CODE   FIRST+: T_LPAR
S_BRACK_EXP   :: T_LBRACK     S_EXP         T_RBRACK                             FIRST+: T_LBRACK 
S_BRACK_EXP   :: T_e                                                             FIRST+: T_COMMA  T_SEMI
S_IDLIST      :: T_COMMA      T_VAR         S_BRACK_EXP   S_IDLIST               FIRST+: T_COMMA
S_IDLIST      :: T_e                                                             FIRST+: T_SEMI
S_PARM_LIST   :: T_TYPE       T_VAR         S_PARM_LIST_A                        FIRST+: T_TYPE
S_PARM_LIST   :: T_VOID       S_PARM_LIST_C                                      FIRST+: T_VOID 
S_PARM_LIST   :: T_e                                                             FIRST+: T_RPAR
S_PARM_LIST_A :: T_COMMA      S_PARM_LIST_B                                      FIRST+: T_COMMA
S_PARM_LIST_A :: T_e                                                             FIRST+: T_RPAR
S_PARM_LIST_B :: S_TYPE       T_VAR         S_PARM_LIST_A                        FIRST+: T_VOID T_TYPE
S_PARM_LIST_C :: T_VAR        S_PARM_LIST_A                                      FIRST+: T_VAR
S_PARM_LIST_C :: T_e                                                             FIRST+: T_RPAR
S_FUNC_DEF    :: T_LBRACE     S_DATA        S_STATEMENT  T_RBRACE                FIRST+: T_LBRAC
S_FUNC_DEF    :: T_SEMI                                                          FIRST+: T_SEMI
S_DATA        :: S_TYPE       T_VAR         S_IDLIST     S_DATA                  FIRST+: T_VOID T_TYPE
S_DATA        :: T_E                                                             FIRST+: T_VAR T_RBRACK
S_STATEMENT   :: T_VAR        S_STATEMENT_A S_STATEMENT                          FIRST+: T_VAR
S_STATEMENT_A :: S_BRACK_EXP  T_EQ          S_EXP        T_SEMI      S_STATMENT  FIRST+: T_LBRACK T_EQ
S_STATENENT_A :: T_LPAR       S_EXP_LIST    T_RPAR       T_SEMI      S_STATE     FIRST+: T_LPAR
S_STATEMENT   :: T_E                                                             FIRST+: T_RBRACE
S_CODE        :: S_FUNC       S_FUNC_DEC    S_CODE                               FIRST+: T_VOID T_TYPE
S_CODE        :: T_e                                                             FIRST+: T_EOF
S_EXP         :: S_TERM       S_EXP_A                                            FIRST+: T_VAR T_NUMBER T_MINUS T_LPAR
S_EXP_A       :: S_ADD        S_TERM        S_EXP_A                              FIRST+: T_ADD T_MINUS
S_EXP_A       :: T_e                                                             FIRST+: Need mroe info
