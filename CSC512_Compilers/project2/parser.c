#ifndef _PARSER_C_
#define _PARSER_C_

#include <stdio.h>
#include "scanner.h"
#include "token.h"
#include "parserTable.h"

#define true  1
#define false 0 

int main( int argc, char ** argp, char ** envp ){

    initLookupTable();
    /** Very lazy way to encode a parser table. 
          but works for here.  **/

    Scanner s = Scanner_new( argp[1], NULL );
    Token tok = Scanner_nextToken( s );
    TokenStack stack = TokenStack_new();
    TokenStack_push( stack, Token_new( T_EOF,  NULL, true ) );
    TokenStack_push( stack, Token_new( S_DATA, NULL, false) );
    #define TOS stack->head
    while( 1 ) {
        
        Token_write( tok, stderr );
        while( isSkip( tok->type ) ) 
        {
            tok = Scanner_nextToken( s );
            Token_write( tok, stderr );
        }

        if( TOS->type == T_EOF && tok->type == T_EOF )
        {
            break;       
        }
        else if( isTerminal(TOS->type) ) 
        {
            if( TOS->type == tok->type )
            {
                Token_free( TokenStack_pop( stack ) );
                tok = Scanner_nextToken( s ); 
            } 
            else 
            {
                fprintf(stderr,"Illegal argument\n");
                exit(1); 
            }
        }
        else 
        {
            int idx = lookup( TOS->type, tok->type );
            if( idx <= 0 ) 
            { 
                fprintf(stderr,"Illegal Argument\n");
                exit(1);
            } 
            else 
            {
                Token_free( TokenStack_pop( stack ) );
                token_t * rule = ruleTable[ idx ];
                while( *rule != T_e )
                {
                    TokenStack_push( stack, Token_new( *rule, "", isTerminal(*rule) ));
                    rule++;
                }
            }
        }      
    }
    printf( "SUCCESS\n" );
}
#endif

