#ifndef _PARSER_C_
#define _PARSER_C_

#include <stdio.h>
#include <unistd.h>
#include "scanner.h"
#include "token.h"
#include "generate.h"
#include "parserTable.h"

#define true  1
#define false 0 

#define TOS stack->head->token

int main( int argc, char ** argp, char ** envp ){

    int data = 0; 
    int func = 0; 
    int stmt = 0;

    initLookupTable();
    /** Very lazy way to encode a parser table. 
          but works for here.  **/

    Scanner s   = Scanner_new( argp[1] );
    Token tok   = Scanner_nextToken( s );
    Token meta  = Token_new( S_START, NULL); 
    Token tree  = Token_new( S_START, NULL);
    Token eof   = Token_new( T_EOF, NULL  );

    TokenStack stack = TokenStack_new();
    TokenStack_push( stack, eof  );
    TokenStack_push( stack, tree );

    int iter = 1;
    int max_iter = 1000000; 
    while( -1 ) {
        //fprintf( stdout, "\nIteration %d\n", iter - 1 ); 
        while( isSkip( tok->type ) ) 
        {
            Token_prependChild( meta, tok ); 
            tok = Scanner_nextToken( s );
        }

        // fprintf( stderr, "TOS -> "); 
        // Token_write( TOS, stderr ); 
        // fprintf( stderr, "TOK -> "); 
        // Token_write( tok, stderr );
        // fprintf( stderr, "STK -> ");
        // TokenStack_write( stack, stderr);

        if( TOS->type == T_EOF && tok->type == T_EOF )
        {
            break;       
        }
        else if( isTerminal(TOS->type) ) 
        {
            if( TOS->type == tok->type )
            {
                Token term = TokenStack_pop( stack );
                term->value = tok->value;
                tok->value = NULL;
                Token_free( tok ); 
                tok  = Scanner_nextToken( s ); 
            } 
            else 
            {
                fprintf(stderr,"Parser error in file %s, line %d\n", argp[1], s->lineNo + 1);      
                fprintf(stderr,"   illegal token %s, transforming %s\n", tokenName[ (int) tok->type ], tokenName[ (int) TOS->type ]); 
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
                fprintf(stderr,"Parser error in file %s, line %d\n", argp[1], s->lineNo + 1);      
                fprintf(stderr,"   illegal token %s, transforming %s\n", tokenName[ (int) tok->type ], tokenName[ (int) TOS->type ]); 
                exit(1);
            } 
            else 
            {
                Token parent = TokenStack_pop( stack );

                
                token_t * rule = ruleTable[ idx ];
                for( int i = RULE_LENGTH-1; i >= 0; i-- ){
                    if( rule[i] == T_e)
                        continue;
                    Token newT = Token_new( rule[i], "" );
                    Token_appendChild( parent, newT ); 
                    TokenStack_push( stack, newT );
                }
            }
        }      
    }
    TokenStack_free( stack ); 
    Scanner_free(s); 
    transform( tree ); 
   
    
    char * newName = malloc( sizeof( char ) * (strlen( argp[1] ) + 5 ) ); 
    strcpy( newName, argp[1]); 
    
    char * itr; 
    for( int i = 0; i < strlen(newName ); i++) 
       if( newName[i] == '.' ) itr = newName + i;
    strcpy(itr, "_gen.c" );
    FILE * fp = fopen( newName, "w" ); 
    generate( fp, meta, tree ); 
    printf("Generated file %s\n", newName ); 
}
#endif

