from __future__ import print_function
import sys
"""
   ***apologies for the mess***
   Helper script to generate the token data so that it can be quickly changed and tested

   MY parser will read in a special syntax file to chage how the rules behave. 
   I have cheated and hacked it for this assignment by placing #includes with generated code snippits 
   at certain points in my code. 

   These places are 
       The definitions of all tokens
       The parser rules
       The rule look up table definition
       A token translation function that takes generic tokens and casts them to a new token if it matches
          some explicit pattern

"""
 
i          = 1
cur_token  = ""
token_list = { 
    "T_COMMENT"  : 1, 
    "T_MCOMMENT" : 1, 
    "T_META"     : 1,
    "T_EOF"      : 1,
    "T_OP"       : 1,
    "T_STRING"   : 1,
    "T_NUMBER"   : 1,
    "T_MULT"     : 1,
    "T_XDATA"    : 1,
    "T_XTYPE"    : 1,
    "T_XPARM"    : 1,
    "S_XDATA"    : 1,
    "S_XCODE"    : 1
}
rules      = []
ptble      = []
casts      = []

for line in open( "syntax.c", "r") :
    line = line.strip()

    if len(line) < 2 :
        continue 
    if line.startswith("//")  : 
        continue
 
    if line.startswith("[CAST]"):
        casts.append( line )
        continue

    arr = line.split() 
    arr.remove("::")
    larr = arr[:arr.index("FIRST+:")]
    rarr = arr[arr.index("FIRST+:") + 1:]
    map( lambda x : token_list.__setitem__(x,1), larr ) 
    larr = larr + ["T_e"] * (7 - len( larr) )
    rules.append(larr)
    ptble.append(rarr)

tokens = sorted(token_list.keys())[::-1]

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
TERM_COUNT = sum([ 1 for x in tokens if x[0] == "T"])
SYM_COUNT  = sum([ 1 for x in tokens if x[0] == "S"])
enums.write("const char * tokenName[];\n" )
enums.write("#define TERM_COUNT %d\n" % TERM_COUNT )
enums.write("#define SYM_COUNT  %d\n" % SYM_COUNT  )

tknc = open ("token.gen.c", "w")
tknc.write("const char * tokenName[] = {\n" )
for x in tokens : tknc.write("    \"%s\",\n" % x )
tknc.write("};\n");

"""
   Generate the rules table 
"""
rfile = open("parserTable.gen.h", "w")
rfile.write("//Rules table\n")
rfile.write("token_t ruleTable [][5]= {\n")
i = 1
for arr in rules : 
    rfile.write( "/* %2d : %-13s */ { %-13s, %-13s, %-13s, %-13s, %-13s, },\n" % (i, arr[0], arr[1], arr[2], arr[3], arr[4], arr[5] ) ) 
    i +=1
rfile.write("};\n")

"""
   Generate the table look up
"""
cfile = open("parserTable.gen.c", "w")
cfile.write("//lookups \n")
for x in xrange( len( rules ) ) :
    for y in ptble[x] :
       cfile.write( "set( %-13s, %-13s, %d );\n"%( rules[x][0],y , x+1 ))


"""
    Generate a token cast file 
"""
casts = [ x.split()[1:] for x in casts ]
sfile = open("scanner.gen.c", "w")
for x in casts : 
    sfile.write( "if( t->type == %s && strcmp( t->value, %s ) == 0 ) t->type = %s;\n" %  tuple(x) )
