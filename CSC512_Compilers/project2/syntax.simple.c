//DATA SEGEMENT
<data>    :: <type> <idList> ";" <data>
          :: e

<idList>  :: <id> <idList'> 
<idList`> :: "," <id> <idList'>
          :: e

<id>      :: <var> <id'>
<id'>     :: "[" <exp> "]"
          :: e 

<type>    :: "int" 
          :: "void" 
          :: "binary" 
          :: "decimal"

//EXPRESSIONS 
<exp>     :: <term> <exp'> 
<exp'>    :: <add> <term> <exp'>
          :: e

       
<term>    :: <fact> <term'>
<term'>   :: <mult> <fact> <term'>
          :: e

<fac>     :: <var> 
          :: <var> "[" <exp> "]"
          //:: <var> "(" <expList> ")"
          :: <number>
          :: "(" <exp> ")"

//TERMINALS
<cmp>    :: "=="|"!="|">" |">="|"<" |"<="
<add>    :: "+" | "-"
<mult>   :: "*" | "/"
<cop>    :: "||"|"&&"
<var>    :: r"[A-Za-z_][A-Za-z0-9_]*"
<number> :: r"[+-]?[1-9][0-9]*"
