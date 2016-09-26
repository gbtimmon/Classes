#ifndef _PARSER_C_
#define _PARSER_C_

#include <stdio.h>
#include <unistd.h>
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
    TokenStack_push( stack, Token_new( T_EOF,  NULL ) );
    TokenStack_push( stack, Token_new( S_DATA, NULL ) );
    #define TOS stack->head
    int iter = 1;
    int max_iter = 1000; 
    while( iter++ < max_iter) {
        fprintf( stderr, "\nIteration %d\n", iter - 1 ); 
        while( isSkip( tok->type ) ) 
        {
            tok = Scanner_nextToken( s );
        }

        fprintf( stderr, "TOS -> "); 
        Token_write( TOS, stderr ); 
        fprintf( stderr, "TOK -> "); 
        Token_write( tok, stderr );
        fprintf( stderr, "STK -> ");
        TokenStack_write( stack, stderr);

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
                fprintf(stderr,"Unexpected Terminal\n");
                exit(1); 
            }
        }
        else 
        {
            int idx = lookup( TOS->type, tok->type );
            fprintf( stderr, "Rule: %d\n", idx + 1 ); 
            if( idx < 0 ) 
            { 
                fprintf(stderr,"Illegal Argument\n");
                exit(1);
            } 
            else 
            {
                Token_free( TokenStack_pop( stack ) );
                token_t * rule = ruleTable[ idx ];
                for( int i = RULE_LENGTH-1; i >= 0; i-- ){
                    if( rule[i] == T_e)
                        continue;
                    Token newT = Token_new( rule[i], "" );
                    TokenStack_push( stack, newT );
                }
            }
        }      
    }
    fprintf( stderr, "SUCCESS\n" );
}
#endif

