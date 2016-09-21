TERMINALS = <cmp> <add> <mult> <cop> <var> <number>

FIRST( data    ) :: <type>, e
FIRST( idList  ) :: <var>
FIRST( idList' ) :: ",", e
FIRST( id      ) :: <var>
FIRST( exp     ) :: <var>, <number>, "("
FIRST( exp'    ) :: <add>, e
FIRST( term    ) :: <var>, <number>, "("
FIRST( term'   ) :: e, <mult>
FIRST( fact    ) :: <var>, <number>, "("

FOLLOW( data   ) :: eof
FOLLOW( idList') :: ";"
FOLLOW( exp'   ) :: ")", "]"
FOLLOW( term'  ) :: <add>, ")", "]"

FIRST+( data   ) :: <type>, eof
FIRST+( idList ) :: <var>
FIRST+( idList') :: ",", ";"
FIRST+( id     ) :: <var> 
FIRST+( exp    ) :: <var>, <number>, "("
FIRST+( exp'   ) :: <add>, ")", "]"
FIRST+( term   ) :: <var>, <number>, "("
FIRST+( term'  ) :: <mult>, <add>, ")", "]"
FIRST+( fact   ) :: <var>, <number>, "("

data     ok.
idList   ok.
idList'  ok.
id       ok.
id'      ok.
exp      ok.
exp'     ok.
term     ok.
term'    ok.
fact     ok.
