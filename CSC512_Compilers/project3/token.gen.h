// Tokens list 
typedef enum _token_t {
    T_e,
    T_XTYPE,
    T_XPARM,
    T_XDATA,
    T_WRITE,
    T_WHILE,
    T_VOID,
    T_VAR,
    T_TYPE,
    T_STRING,
    T_SEMI,
    T_RPAR,
    T_RETURN,
    T_READ,
    T_RCURL,
    T_RBRACK,
    T_PRINT,
    T_PLUS,
    T_OP,
    T_NUMBER,
    T_MULT,
    T_MINUS,
    T_META,
    T_MCOMMENT,
    T_LPAR,
    T_LCURL,
    T_LBRACK,
    T_IF,
    T_EQ,
    T_EOF,
    T_CONTINUE,
    T_COMMENT,
    T_COMMA,
    T_CMP,
    T_BREAK,
    T_BOOL_OP,
    S_XFUNC,
    S_XDATA,
    S_XCODE,
    S_TYPE,
    S_TERM_A,
    S_TERM,
    S_STATEMENT_R,
    S_STATEMENT_P,
    S_STATEMENT_D,
    S_STATEMENT_C,
    S_STATEMENT_B,
    S_STATEMENT_A,
    S_STATEMENT,
    S_START_A,
    S_START,
    S_PARM_LIST_C,
    S_PARM_LIST_B,
    S_PARM_LIST_A,
    S_PARM_LIST,
    S_IDLIST,
    S_FUNC_DEF,
    S_FUNC,
    S_FACT_A,
    S_FACT,
    S_EXP_LIST_B,
    S_EXP_LIST_A,
    S_EXP_LIST,
    S_EXP_A,
    S_EXP,
    S_DATA,
    S_COND_EXP_A,
    S_COND_EXP,
    S_COND,
    S_CODE,
    S_BRACK_EXP,
    S_BLOCK,
    S_ADD,
} token_t;

//name mapping
const char * tokenName[];
#define TERM_COUNT 36
#define SYM_COUNT  37
