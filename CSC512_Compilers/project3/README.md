This is the code for my compiler / code generator. 
The compile works in a three step process - the parser generates a tree, transform.c transforms the tree into a better form for output and generator.c takes the tree and outputs c code. 

A few small changes to token from the last project. I added functions for creating and manipulating trees of token, and pointers to support trees. I am lazy so the parser tokens are directly used to create a AST tree.

The tree after transformation has a simple structure illistrated below. 

START
  +- ROOT DATA
  +- FUNC 1
  |    +- type
  |    +- name
  |    +- parms
  |    |   +-parm 1
  |    |   ...
  |    +- code
  |       +- stmnt 1
  |       +- stmnt 2 (if , or while)
  |          +-code 
  |             +stmnt 1
  |
  +- FUNC 2
  +- FUNC 3

Trasnformations happen in a depth first search manner. This is important becuase a transform function is written for each node, and it is written with the assumption that all of its child nodes are fully transformed. Also the recursive DFS implementation allows us to change the tree in place without worrying about losing the order to process the tokens since the order is already fully in the stack before any transfromations occur. 

Each root and function node has a symbol table. Which corresponds to local and global scope. There is 
no support for more than the two scopes. 

 ----------------------------------
(README from project 2 is bleow )

This is the code for my parser. 

This code code has a few class

 1.   *Buffer*  

      A buffer for storing string data. A convience class for storing data
      read by the scanner from the input file with efficent and easy memory mangement. 

 2.   *Scanner*

      Scanner class will read the input file and generate tokens. The tokens are generally 
      just the tokens from project one, excpet there is a dynamically generated function 
      ( scanner.gen.c ) that will cast generic tokens to syntax specific language. This 
      gives me easy debugging of the syntax. 

 3.   *Token*  

      Define the tokens available and defines function for creation and desctruction of 
      tokens. Token ID are unique to token type and dynamically generated in the file 
      ( token.gen.h ). A helper class to generate string names from internal IDs is includes
      and generated in ( token.gen.c ). Not needed in live code but it is useful for debugging. 
      Its important to note the in the c standard, a enum which does not specify defualt will 
      assing the value 0 to the first enum value and increment by one per enum. Should be
      completely portable. 

 4.   *ParserTable*

      This file offers the lookups for Parse rules and the rules table. Almost all of the code here
      is generated dynamically and in the files ( parserTable.gen.c, parserTable.gen.h ). There is an
      init file that loads the rule table into memory -- mostly because it was to complicated for me to 
      get literals to work easily with the index of each token being dynamic.

 5.   *Parser*
 
      The main function is here, it expects the file to be parsed to be passed in a parameter, I did not
      take the time to make it accept std in as a argument since it was not part of the assignment though
      changing the scanner to default to stdin on a NULL filename input would be a simple fix. 

 6.   *syntax*
      
      This is where the rules are defined. This file is read by tableGenerator.py to generate all of the
      dynamically generated code that allows me to change the rules table with minimal effort.
      Not a c file at all, but I like the .c since it makes vim higligh my comments. 
      [CAST] directives here change a token of one type with a certain value to another token type. 

 7.   *generateTable.py*
       Dynamic code generator. Dirty code .... Im sorry I dont have time to clean it up. 
       **MUST** be run before the code will compile as the .gen files will not be created until it is
       run.  As long as syntax.c does not change the .gen files dont need to be updated however. 

 8.    *makefile*
       type make -B. I recommend -B since Im not very good at make and couldnt figure out how to force it
       to always run the generation script. -B force fresh gen files so changes to syntax.c arent ignored. 






    
