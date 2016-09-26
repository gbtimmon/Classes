S_START       :: S_DATA       S_CODE   
S_DATA        :: T_TYPE       S_IDLIST      T_SEMI        S_DATA
              :: T_VOID       S_IDLIST      T_SEMI        S_DATA
              :: e 
S_IDLIST      :: S_ID         S_IDLIST_A           
S_IDLIST_A    :: T_COMMA      S_ID          S_IDLIST_A
S_ID          :: T_VAR        S_ID_A                
S_ID_A        :: T_LBRACK     S_EXP         T_RBRACK
              :: e
S_EXP         :: S_TERM       S_EXP_A     
S_EXP_A       :: T_ADD        S_EXP_B
              :: T_MINUS      S_EXP_B
              :: e
S_EXP_B       :: S_TERM       S_EXP_A
S_TERM        :: S_FACT       S_TERM_A      
              :: T_MULT       S_FACT        S_TERM_A
              :: e
S_FACT        :: T_VAR        S_FACT_A 
              :: T_NUMBER    
              :: T_MINUS      T_NUMBER 
              :: T_LPAR       S_EXP         T_RPAR
S_FACT_A      :: T_LBRACK     S_EXP         T_RBRACK
              :: T_LPAR       S_EXP         S_EXP_LIST     T_RPAR
              :: e
S_EXP_LIST    :: T_COMMA      S_EXP         S_EXP_LIST
              :: e
S_CODE        :: S_FUNC       S_FUNC_DEF    S_CODE 
              :: e                    
S_FUNC        :: T_TYPE       T_VAR         T_LPAR        S_PARM_LIST     T_RPAR
S_FUNC_DEF    :: T_LBRACE     S_DATA        S_STMNTL      T_RBRACE
              :: T_SEMI
S_PARM_LIST   :: T_TYPE       T_VAR         S_PARM_LIST_A
              :: T_vOID       S_PARM_LIST_C                  
              :: e   
S_PARM_LIST_A :: T_COMMA      S_PARM_LIST_B
              :: e
S_PARM_LIST_B :: T_TYPE       T_VAR         S_PARM_LIST_A
              :: T_VOID       T_VAR         S_PARM_LIST_A
S_PARM_LIST_C :: T_VAR        S_PARM_LIST_A
              :: e
S_BLOCK       :: T_LBRACE     S_STMNT_LIST  T_RBRACE 
S_STMNT_LIST  :: S_STMNT      S_STMNT_LIST 
              :: e
S_STMNT       :: S_VAR        S_STMNT_A 
              :: S_IF       
              :: S_WHILE    
              :: S_RETURN   
              :: S_BREAK    
              :: S_CONTINUE 
              :: T_READ       T_LPAR        T_VAR         T_RPAR           T_SEMI 
              :: T_WRITE      T_LPAR        S_EXP         T_RPAR           T_SEMI 
              :: T_PRINT      T_LPAR        T_STRING      T_RPAR           T_SEMI 
S_STMNT_A     :: S_ASSIGN
              :: T_LBRACK     S_EXP         T_RBRACK      S_ASSIGN
              :: T_LPAR       S_EXP_LIST    T_RPAR        T_SEMI 
S_ASSIGN      :: T_EQ         S_EXP         T_SEMI 
S_IF          :: T_IF         T_LPAR        S_COND_EXP    T_RPAR           S_BLOCK 
S_COND_EXP    :: S_COND       T_COND_EXP_A
S_COND_EXP_A  :: T_CONDOP     S_COND 
              :: e 
S_COND        :: S_EXP        T_CMP         S_EXP 
S_WHILE       :: T_WHILE      T_LPAR        S_COND_EXP    T_RPAR           S_BLOCK 
S_RETURN      :: T_RETURN     S_RETURN_A 
S_RETURN_A    :: S_EXP        T_SEMI
              :: T_SEMI
S_BREAK       :: T_BREAK      T_SEMI 
S_CONTINUE    :: T_CONT       T_SEMI

//TERMINALS
T_COMMA  = ","
T_SEMI   = ";"
T_LPAR   = "("
T_RPAR   = ")"
T_LBRACK = "["
T_RBRACK = "]"
T_LBRACE = "{"
T_RBRACE = "}"
T_CONDOP = "||", "&&"
T_EQ     = "="
T_MULT   = "*" , "/"
T_CMP    = "==", "!=", ">=", ">", "<=", "<"
T_ADD    = "+" 
T_MINUS  = "-"
T_VAR    = and non key word stating with [a-zA-z_]
T_STRING = any string. 
T_TYPE   = "int", "binary", "decimal" 
T_VOID   = "void"
T_IF     = "if"
T_WHILE  = "while"
T_RETURN = "return"
T_BREAK  = "break"
T_CONT   = "continue"
T_READ   = "read"
T_WRITE  = "write"
T_PRINT  = "print"
T_NUMBER = Any number.
