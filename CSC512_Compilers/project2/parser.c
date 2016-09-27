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

    int data = 0; 
    int func = 0; 
    int stmt = 0;

    initLookupTable();
    /** Very lazy way to encode a parser table. 
          but works for here.  **/

    Scanner s = Scanner_new( argp[1] );
    Token tok = Scanner_nextToken( s );
    TokenStack stack = TokenStack_new();
    TokenStack_push( stack, Token_new( T_EOF,   NULL ) );
    TokenStack_push( stack, Token_new( S_START, NULL ) );
    #define TOS stack->head
    int iter = 1;
    int max_iter = 1000; 
    while( iter++ < max_iter) {
        //fprintf( stderr, "\nIteration %d\n", iter - 1 ); 
        while( isSkip( tok->type ) ) 
        {
            tok = Scanner_nextToken( s );
        }

        //fprintf( stderr, "TOS -> "); 
        //Token_write( TOS, stderr ); 
        //fprintf( stderr, "TOK -> "); 
        //Token_write( tok, stderr );
        //fprintf( stderr, "STK -> ");
        //TokenStack_write( stack, stderr);

        if( TOS->type == T_EOF && tok->type == T_EOF )
        {
            break;       
        }
        else if( isTerminal(TOS->type) ) 
        {
            if( TOS->type == tok->type )
            {
                Token_free(TokenStack_pop( stack ));
                //fprintf( stdout, "%s\n", tok->value );
                Token_free(tok);
                tok = Scanner_nextToken( s ); 
            } 
            else 
            {
                fprintf(stderr,"error\n");
                exit(1); 
            }
        }
        else 
        {
            int idx = lookup( TOS->type, tok->type );
            int cidx = idx + 1; //Corrected to match the parser table. 
            if ( cidx == 7  || cidx == 11 || cidx == 24 )
                data++;

            if ( cidx == 26 || cidx == 29 || cidx == 30  
              || cidx == 32 || cidx == 35 || cidx == 37 
              || cidx == 38 || cidx == 39 || cidx == 41 )
                stmt++;

            if ( cidx == 22 )
                func++;
                
 
            
            //fprintf( stderr, "Rule: %d\n", idx + 1 ); 
            if( idx < 0 ) 
            { 
                fprintf(stdout,"error\n");
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
    fprintf( stdout, "pass\n" );
    fprintf( stdout, "variable %d function %d statement %d\n", data, func, stmt );
}
#endif

