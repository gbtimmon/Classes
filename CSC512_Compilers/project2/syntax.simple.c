//DATA SEGEMENT
<data>    :: <type> <idList> "semi" <data>
          :: e
<idList>  :: <id> <idList'> 
<idList`> :: "comma" <id> <idList'>
          :: e
<id>      :: "var" <id'>
<id'>     :: "lBrac" <exp> "rBrac"
          :: e 
<exp>     :: <term> <exp'> 
<exp'>    :: "add" <term> <exp'>
          :: e
<term>    :: <fact> <term'>
<term'>   :: "mult" <fact> <term'>
          :: e
<fact>    :: "var" <fact'>
          :: "number"
          :: "lPar" <exp> "rPar"
<fact'>   :: "lBrac" <exp> "rBrac"
          :: e
//TERMINALS
<cmp>    :: "=="|"!="|">" |">="|"<" |"<="
<add>    :: "+" | "-"
<mult>   :: "*" | "/"
<cop>    :: "||"|"&&"
<var>    :: r"[A-Za-z_][A-Za-z0-9_]*"
<number> :: r"[+-]?[1-9][0-9]*"
