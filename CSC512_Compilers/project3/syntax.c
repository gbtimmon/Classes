S_TYPE        :: T_TYPE                                                          FIRST+: T_TYPE 
S_TYPE        :: T_VOID                                                          FIRST+: T_VOID
S_ADD         :: T_PLUS                                                          FIRST+: T_PLUS
S_ADD         :: T_MINUS                                                         FIRST+: T_MINUS
S_START       :: S_TYPE       T_VAR         S_START_A                            FIRST+: T_TYPE   T_VOID
S_START       :: T_e                                                             FIRST+: T_EOF
S_START_A     :: S_BRACK_EXP  S_IDLIST      T_SEMI        S_START                FIRST+: T_LBRACK T_COMMA T_SEMI
S_START_A     :: T_LPAR       S_PARM_LIST   T_RPAR        S_FUNC_DEF    S_CODE   FIRST+: T_LPAR
S_BRACK_EXP   :: T_LBRACK     S_EXP         T_RBRACK                             FIRST+: T_EQ T_LBRACK 
S_BRACK_EXP   :: T_e                                                             FIRST+: T_COMMA  T_SEMI T_RPAR T_BOOL_OP T_EQ T_LPAR T_RPAR T_MINUS T_MULT T_CMP T_PLUS T_RBRACK
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
S_FUNC        :: S_TYPE       T_VAR         T_LPAR       S_PARM_LIST T_RPAR      FIRST+: T_VOID T_TYPE
S_FUNC_DEF    :: T_LCURL      S_DATA        S_STATEMENT  T_RCURL                 FIRST+: T_LCURL
S_FUNC_DEF    :: T_SEMI                                                          FIRST+: T_SEMI
S_DATA        :: S_TYPE       T_VAR        S_DATA_A                              FIRST+: T_VOID T_TYPE
S_DATA        :: T_e                                                             FIRST+: T_VAR T_RCURL T_WHILE T_IF T_RETURN T_BREAK T_CONTINUE T_READ T_WRITE T_PRINT
S_DATA_A      :: S_BRACK_EXP  S_IDLIST     T_SEMI      S_DATA                    FIRST+: T_LBRACK T_COMMA T_SEMI
// Assign or Func call
S_STATEMENT   :: T_VAR        S_STATEMENT_A S_STATEMENT                          FIRST+: T_VAR
S_STATEMENT_A :: S_BRACK_EXP  T_EQ          S_EXP        T_SEMI                  FIRST+: T_LBRACK T_EQ
S_STATEMENT_A :: T_LPAR       S_EXP_LIST    T_RPAR       T_SEMI                  FIRST+: T_LPAR
// While or if
S_STATEMENT   :: T_WHILE      S_STATEMENT_B S_STATEMENT                          FIRST+: T_WHILE
S_STATEMENT   :: T_IF         S_STATEMENT_B S_STATEMENT                          FIRST+: T_IF
S_STATEMENT_B :: T_LPAR       S_COND_EXP    T_RPAR       S_BLOCK                 FIRST+: T_LPAR
// return 
S_STATEMENT   :: T_RETURN     S_STATEMENT_R T_SEMI       S_STATEMENT             FIRST+: T_RETURN
S_STATEMENT_R :: S_EXP                                                           FIRST+: T_VAR T_NUMBER T_MINUS T_LPAR
S_STATEMENT_R :: T_e                                                             FIRST+: T_SEMI
// read
S_STATEMENT   :: T_READ       S_STATEMENT_C T_SEMI       S_STATEMENT             FIRST+: T_READ
S_STATEMENT_C :: T_LPAR       T_VAR         S_BRACK_EXP  T_RPAR                  FIRST+: T_LPAR
// break or continue
S_STATEMENT   :: T_BREAK      T_SEMI        S_STATEMENT                          FIRST+: T_BREAK
S_STATEMENT   :: T_CONTINUE   T_SEMI        S_STATEMENT                          FIRST+: T_CONTINUE
// write
S_STATEMENT   :: T_WRITE      S_STATEMENT_D T_SEMI       S_STATEMENT             FIRST+: T_WRITE
S_STATEMENT_D :: T_LPAR       S_EXP         T_RPAR                               FIRST+: T_LPAR
// print 
S_STATEMENT   :: T_PRINT      S_STATEMENT_P T_SEMI       S_STATEMENT             FIRST+: T_PRINT
S_STATEMENT_P :: T_LPAR       T_STRING      T_RPAR                               FIRST+: T_LPAR
S_STATEMENT   :: T_e                                                             FIRST+: T_RCURL
S_CODE        :: S_FUNC       S_FUNC_DEF    S_CODE                               FIRST+: T_VOID T_TYPE
S_CODE        :: T_e                                                             FIRST+: T_EOF
S_EXP         :: S_TERM       S_EXP_A                                            FIRST+: T_VAR T_NUMBER T_MINUS T_LPAR
S_EXP_A       :: S_ADD        S_TERM        S_EXP_A                              FIRST+: T_PLUS T_MINUS
S_EXP_A       :: T_e                                                             FIRST+: T_RPAR T_SEMI T_RBRACK T_CMP T_BOOL_OP T_COMMA
S_TERM        :: S_FACT       S_TERM_A                                           FIRST+: T_VAR T_LPAR T_NUMBER T_MINUS                        
S_TERM_A      :: T_MULT       S_FACT        S_TERM_A                             FIRST+: T_MULT
S_TERM_A      :: T_e                                                             FIRST+: T_PLUS T_MINUS T_CMP T_RPAR T_BOOL_OP T_SEMI T_RBRACK T_COMMA
S_FACT        :: T_VAR        S_FACT_A                                           FIRST+: T_VAR
S_FACT_A      :: S_BRACK_EXP                                                     FIRST+: T_LBRACK
S_FACT_A      :: T_LPAR       S_EXP_LIST    T_RPAR                               FIRST+: T_LPAR
S_FACT_A      :: T_e                                                             FIRST+: T_RPAR T_BOOL_OP T_SEMI T_MINUS T_PLUS T_MULT T_CMP T_RBRACK T_COMMA
S_FACT        :: T_NUMBER                                                        FIRST+: T_NUMBER
S_FACT        :: T_MINUS      T_NUMBER                                           FIRST+: T_MINUS
S_FACT        :: T_LPAR       S_EXP         T_RPAR                               FIRST+: T_LPAR
//COND 
S_COND_EXP    :: S_COND       S_COND_EXP_A                                       FIRST+: T_NUMBER T_VAR T_LPAR T_MINUS
S_COND_EXP_A  :: T_BOOL_OP    S_COND                                             FIRST+: T_BOOL_OP
S_COND_EXP_A  :: T_e                                                             FIRST+: T_RPAR
S_COND        :: S_EXP        T_CMP         S_EXP                                FIRST+: T_NUMBER T_VAR T_LPAR T_MINUS
S_BLOCK       :: T_LCURL      S_STATEMENT   T_RCURL                              FIRST+: T_LCURL
S_EXP_LIST    :: S_EXP        S_EXP_LIST_A                                       FIRST+: T_VAR T_NUMBER T_MINUS T_LPAR
S_EXP_LIST    :: T_STRING     S_EXP_LIST_A                                       FIRST+: T_STRING
S_EXP_LIST    :: T_e                                                             FIRST+: T_RPAR
S_EXP_LIST_A  :: T_COMMA      S_EXP_LIST_B                                       FIRST+: T_COMMA
S_EXP_LIST_A  :: T_e                                                             FIRST+: T_RPAR
S_EXP_LIST_B  :: S_EXP        S_EXP_LIST_A                                       FIRST+: T_VAR T_NUMBER T_MINUS T_LPAR
S_EXP_LIST_B  :: T_STRING     S_EXP_LIST_A                                       FIRST+: T_STRING
 
[CAST]  T_VAR  "int"      T_TYPE
[CAST]  T_VAR  "void"     T_VOID
[CAST]  T_VAR  "decimal"  T_TYPE
[CAST]  T_VAR  "binary"   T_TYPE
[CAST]  T_VAR  "read"     T_READ
[CAST]  T_VAR  "write"    T_WRITE
[CAST]  T_VAR  "while"    T_WHILE
[CAST]  T_VAR  "break"    T_BREAK
[CAST]  T_VAR  "return"   T_RETURN
[CAST]  T_VAR  "print"    T_PRINT
[CAST]  T_VAR  "continue" T_CONTINUE
[CAST]  T_VAR  "if"       T_IF
[CAST]  T_OP   "="        T_EQ
[CAST]  T_OP   "+"        T_PLUS
[CAST]  T_OP   "-"        T_MINUS
[CAST]  T_OP   "*"        T_MULT
[CAST]  T_OP   "/"        T_MULT
[CAST]  T_OP   "("        T_LPAR
[CAST]  T_OP   ")"        T_RPAR
[CAST]  T_OP   "["        T_LBRACK
[CAST]  T_OP   "]"        T_RBRACK
[CAST]  T_OP   "{"        T_LCURL
[CAST]  T_OP   "}"        T_RCURL
[CAST]  T_OP   ","        T_COMMA
[CAST]  T_OP   ";"        T_SEMI
[CAST]  T_OP   "||"       T_BOOL_OP
[CAST]  T_OP   "&&"       T_BOOL_OP
[CAST]  T_OP   ">"        T_CMP
[CAST]  T_OP   ">="       T_CMP
[CAST]  T_OP   "<"        T_CMP
[CAST]  T_OP   "<="       T_CMP
[CAST]  T_OP   "=="      T_CMP
[CAST]  T_OP   "!="      T_CMP
