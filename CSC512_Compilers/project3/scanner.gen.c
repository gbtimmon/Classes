if( t->type == T_VAR && strcmp( t->value, "int" ) == 0 ) t->type = T_TYPE;
if( t->type == T_VAR && strcmp( t->value, "void" ) == 0 ) t->type = T_VOID;
if( t->type == T_VAR && strcmp( t->value, "decimal" ) == 0 ) t->type = T_TYPE;
if( t->type == T_VAR && strcmp( t->value, "binary" ) == 0 ) t->type = T_TYPE;
if( t->type == T_VAR && strcmp( t->value, "read" ) == 0 ) t->type = T_READ;
if( t->type == T_VAR && strcmp( t->value, "write" ) == 0 ) t->type = T_WRITE;
if( t->type == T_VAR && strcmp( t->value, "while" ) == 0 ) t->type = T_WHILE;
if( t->type == T_VAR && strcmp( t->value, "break" ) == 0 ) t->type = T_BREAK;
if( t->type == T_VAR && strcmp( t->value, "return" ) == 0 ) t->type = T_RETURN;
if( t->type == T_VAR && strcmp( t->value, "print" ) == 0 ) t->type = T_PRINT;
if( t->type == T_VAR && strcmp( t->value, "continue" ) == 0 ) t->type = T_CONTINUE;
if( t->type == T_VAR && strcmp( t->value, "if" ) == 0 ) t->type = T_IF;
if( t->type == T_OP && strcmp( t->value, "=" ) == 0 ) t->type = T_EQ;
if( t->type == T_OP && strcmp( t->value, "+" ) == 0 ) t->type = T_PLUS;
if( t->type == T_OP && strcmp( t->value, "-" ) == 0 ) t->type = T_MINUS;
if( t->type == T_OP && strcmp( t->value, "*" ) == 0 ) t->type = T_MULT;
if( t->type == T_OP && strcmp( t->value, "/" ) == 0 ) t->type = T_MULT;
if( t->type == T_OP && strcmp( t->value, "(" ) == 0 ) t->type = T_LPAR;
if( t->type == T_OP && strcmp( t->value, ")" ) == 0 ) t->type = T_RPAR;
if( t->type == T_OP && strcmp( t->value, "[" ) == 0 ) t->type = T_LBRACK;
if( t->type == T_OP && strcmp( t->value, "]" ) == 0 ) t->type = T_RBRACK;
if( t->type == T_OP && strcmp( t->value, "{" ) == 0 ) t->type = T_LCURL;
if( t->type == T_OP && strcmp( t->value, "}" ) == 0 ) t->type = T_RCURL;
if( t->type == T_OP && strcmp( t->value, "," ) == 0 ) t->type = T_COMMA;
if( t->type == T_OP && strcmp( t->value, ";" ) == 0 ) t->type = T_SEMI;
if( t->type == T_OP && strcmp( t->value, "||" ) == 0 ) t->type = T_BOOL_OP;
if( t->type == T_OP && strcmp( t->value, "&&" ) == 0 ) t->type = T_BOOL_OP;
if( t->type == T_OP && strcmp( t->value, ">" ) == 0 ) t->type = T_CMP;
if( t->type == T_OP && strcmp( t->value, ">=" ) == 0 ) t->type = T_CMP;
if( t->type == T_OP && strcmp( t->value, "<" ) == 0 ) t->type = T_CMP;
if( t->type == T_OP && strcmp( t->value, "<=" ) == 0 ) t->type = T_CMP;
if( t->type == T_OP && strcmp( t->value, "==" ) == 0 ) t->type = T_CMP;
if( t->type == T_OP && strcmp( t->value, "!=" ) == 0 ) t->type = T_CMP;
