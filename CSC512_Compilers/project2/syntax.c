//TOP LEVEL
<prog>   :: <data><code>     //top level program - Data then functions. 

//DATA SEGEMENT
<data>    :: <type> <idList> ";" <data>
          :: e

<idList>  :: <id> <idList'> 
<idList`> :: "," <id> <idList'>
          :: e

<id>      :: ID 
          :: ID "[" <exp> "]"

<type>    :: "int" 
          :: "void" 
          :: "binary" 
          :: "decimal"

//CODE SEGMENT
<code>    :: <func> <code>
          :: e

<func>    :: <fDecl> ";"
          :: <fDecl> "{" <data> <stmnts> "}"

<fDecl>   :: <type> <var> "(" <pList> ")"

<pList>   :: "void"
          :: <type> <var> <pList'>
          :: e
<pList'>  :: "," <type> <var> <pList'>
          :: e 

//STATEMENTS 
<stmnts>  :: <stmnt> <stmnts>
          :: e 

<stmnt>   :: <assign>
          :: <call>
          :: <if>
          :: <while>
          :: <return>
          :: <break>
          :: <continue>
          :: "read(" <var> ");"
          :: "write(" <exp> ");"
          :: "print(" <string> ");"

<assign>  :: <id> "=" <exp> ";"

<call>    :: <var>"("<expList>");"

<expList> :: <exp> <expList'>
          :: e

<expList'>:: "," <exp> <expList'>
          :: e

<block>   :: "{" <stmnts> "}"

//CONDITION
<condExp> :: <cond> 
          :: <cond> <cop> <cond>

<cond>    :: <exp> <cmp> <exp>

//CONTROL
<if>      :: "if(" <condExp> ")" <block>
<while>   :: "while(" <condExp> ")" <block>
<return>  :: "return" <exp>;
          :: "return;"
<break>   :: "break;"
<continue>:: "continue;"


//EXPRESSIONS 
<exp>     :: <term> <exp'> 
<exp'>    :: <add> <term> <exp'>
          :: e

       
<term>    :: <fact> <term'>
<term'>   :: <mult> <fact> <term'>
          :: e

<fac>     :: <var> 
          :: <var> "[" <exp> "]"
          :: <var> "(" <expList> ")"
          :: <number>
          :: "(" <exp> ")"

//TERMINALS
<cmp>    :: "=="|"!="|">" |">="|"<" |"<="
<add>    :: "+" | "-"
<mult>   :: "*" | "/"
<cop>    :: "||"|"&&"
<var>    :: r"[A-Za-z_][A-Za-z0-9_]*"
<number> :: r"[+-]?[1-9][0-9]*"
