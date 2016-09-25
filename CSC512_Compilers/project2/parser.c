#ifndef _PARSER_C_
#define _PARSER_C_

#include <stdio.h>
#include "scanner.h"
#include "token.h"

int main( int argc, char ** argp, char ** envp ){
    fprintf(stdout, "%s\n", argp[1] );
    fflush(NULL);
    Scanner s = Scanner_new( argp[1], NULL );
    while( 1 ) {
       
        Token t = Scanner_nextToken( s );
        Token_write( t, stdout );
        if( t->type == T_EOF ) 
            break;
    }
}
#endif

