#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eString.h"

#define BUFFER_START 100


static const char* C_END  = "end";
static const char* C_ADD  = "add";
static const char* C_FIND = "find";
static const char* C_DEL  = "del";

static const char* M_FIRST_FIT = "--first-fit"; 
static const char* M_BEST_FIT  = "--best-fit"; 
static const char* M_WORST_FIT = "--worst-fit"; 

int main(int argc, char** argv)
{
 
    const char* mode = argv[1]; 

    if(   strcmp(mode, M_FIRST_FIT) != 0
      &&  strcmp(mode, M_BEST_FIT)  != 0
      &&  strcmp(mode, M_WORST_FIT) != 0 ) {
        fprintf(stderr, "illeagal argument! %s is not recognized\n", s[1]); 
        exit(1); 
    }

    char   *str; //String, and token.  
    while ((str = chomp(get_line(stdin)))) {

        size_t num_toks; 
        char** s = split(str, " ", &num_toks); 
        
        if       ( strcmp( *s, C_END) == 0 ) { 
            exit(0);    
        }else if ( strcmp( *s, C_ADD) == 0 ) { 
            puts("add\n"); 
        }else if ( strcmp( *s, C_FIND) == 0 ) { 
            puts("find\n"); 
        }else if ( strcmp( *s, C_DEL) == 0 ) { 
            puts("del\n"); 
        } else {
            for(size_t i = 0; i < num_toks; i++){
	        printf("E[%s],", s[i]); 
            }
        }
    }
    return 0;
}
