#ifndef _SUBSTRING_C_
#define _SUBSTRING_C_

#include <stdio.h>

/**
 * Letter tree 
 **/

typedef struct _letter {
   char letter;
   struct _letter[26] child; 
} * Letter;

Letter Letter_new( char letter ){
    Letter n      = malloc( sizeof( struct _letter ) ); 
    n->letter     = letter; 
    n->Child      = memcp 
    
    return n;
};


   
char * bufa[2056], bufb[2056];

int main( int agrc, char* argp, char* envp ) { 

    scanf( "%s", bufa); 
    scanf( "%s", bufb); 

    if( strlen(bufa) < 1 || strlen(bufb) < 1 ) {
        fprintf(stderr, "Improper inputs, exiting\n");
        exit(1); 
    }
  
    /**CODEGOLF! sorry this isnt readable, but I was ahving fun. ;) **/
    for( char * outer = bufa; *outer != '\0'; outer++ ) 
    for( char * inner = bufb; *inner != '\0'; inner++ ) {
        int so = 0; 
        while( outer[so] == inner[so] && outer[so] != '\0' ) so++;
        if( so > 0 ) printf( "%s, %d, %.*s\n", outer, so,  so, outer);
    } 
}
#endif

