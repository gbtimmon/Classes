from __future__ import print_function
import sys
"""
   ***apologies for the mess***
   Helper script to generate the token data so that it can be quickly changed and tested
"""
 
i          = 1
cur_token  = ""
token_list = { 
    "T_COMMENT"  : 1, 
    "T_MCOMMENT" : 1, 
    "T_META"     : 1,
    "T_EOF"      : 1,
    "T_OP"       : 1
}
rules      = []
ptble      = []

for line in open( "syntax.c", "r") :
    line = line.strip()

    if len(line) < 2 :
        continue 
    if line.startswith("//")  : 
        continue

    arr = line.split() 
    arr.remove("::")
    larr = arr[:arr.index("FIRST+:")]
    rarr = arr[arr.index("FIRST+:") + 1:]
    map( lambda x : token_list.__setitem__(x,1), arr ) 
    larr = larr + ["T_e"] * (7 - len( larr) )
    rules.append(larr)
    ptble.append(rarr)

tokens = sorted(token_list.keys())

"""
   Generate the tokens metadata.
"""
enums = open("token.gen.h", "w")
enums.write("// Tokens list \n")
enums.write("typedef enum _token_t {\n")
for x in tokens : enums.write("    %s,\n" % x )
enums.write("} token_t;\n")
enums.write("\n");
enums.write("//name mapping\n")
enums.write("const char * tokenName[] = {\n" )
for x in tokens : enums.write("    \"%s\",\n" % x )
enums.write("};\n");
TERM_COUNT = sum([ 1 for x in tokens if x[0] == "T"])
SYM_COUNT  = sum([ 1 for x in tokens if x[0] == "S"])
enums.write("#define TERM_COUNT %d\n" % TERM_COUNT )
enums.write("#define SYM_COUNT  %d\n" % SYM_COUNT  )

"""
   Generate the rules table 
"""
rfile = open("parserTable.gen.h", "w")
rfile.write("//Rules table\n")
rfile.write("token_t rulesTable [][5]= {\n")
i = 1
for arr in rules : 
    rfile.write( "/* %2d : %-13s */ { %-13s, %-13s, %-13s, %-13s, %-13s, },\n" % (i, arr[0], arr[1], arr[2], arr[3], arr[4], arr[5] ) ) 
    i +=1
rfile.write("};\n")
