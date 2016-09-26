#ifndef _PARSER_C_
#define _PARSER_C_

#include <stdio.h>
#include "scanner.h"
#include "token.h"

#define true  1
#define false 0 

int main( int argc, char ** argp, char ** envp ){

    /** Very lazy way to encode a parser table. 
          but works for here.  **/

    fprintf( stderr, "ENTER LOOP\n");   
    Scanner s = Scanner_new( argp[1], NULL );
    TokenStack stack = TokenStack_new();
    TokenStack_push( stack, Token_new( T_EOF,  NULL, true ) );
    TokenStack_push( stack, Token_new( S_DATA, NULL, false) );
    fprintf( stderr, "ENTER LOOP\n");   
    while( 1 ) {
        
        Token t = Scanner_nextToken( s );
        Token_write( t, stdout );
        if( t->type == T_EOF ) 
            break;
    }
}
#endif

