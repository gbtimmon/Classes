#ifndef _SUBSTRING_C_
#define _SUBSTRING_C_

#include <stdio.h>
#include <string.h>

/**
 * Letter tree 
 **/

enum side_t { LEFT, RIGHT }; 

typedef struct _letter {

   int depth;
   int  left;
   int  right; 
   struct _letter *child[26]; 

} * Letter;

Letter Letter_new( int d ){

    Letter n  = malloc( sizeof( struct _letter ) ); 
    n->depth  = d; 
    n->left   = 0; 
    n->right  = 0; 

    for( int i = 0; i < 26; i++) 
        n->child[i] = NULL;

    return n;
};

Letter Letter_getChild( Letter l, char c) {

    int idx = tolower(c) - ((int) 'a');

    if( l->child[ idx ] == NULL ) {
        l->child[ idx ] = Letter_new(l->depth + 1);
    }

    return l->child[idx];
};

Letter Letter_increment( Letter c, enum side_t s){

    if( s == LEFT ) 
        c->left++;
    else
        c->right++;
    return c; 
};

Letter Letter_print( Letter l, int letter ) { 
    if( letter > 0 )
        printf("--- %c ---\n", (char) 'a' + letter);
    else
        printf("---- HEAD ---\n");
    printf("  depth : %d\n", l->depth);
    printf("  left  : %d\n", l->left);
    printf("  right : %d\n", l->right);
    printf("  pntrs : [");
    for ( int i = 0; i < 26; i++ ) 
        printf( (l->child[i] == NULL) ? " _" : " X");

    printf(" ]\n");
    printf("          [ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ]\n");
    return l; 
};
Letter Letter_addSubstring( Letter l, enum side_t side, char * string ) {

    if( strlen( string ) == 0 )
        return; 

    char cur = string[0];
    Letter cl = Letter_getChild( l, cur );
    Letter_increment( cl, side);
    Letter_addSubstring( cl, side, string + 1);
    return l;
};

int Letter_countSubstrings( Letter l , int letter, int depth) {

    //Letter_print(l, letter);
    if( l->left == 0 || l->right == 0 )
        return 0;
 
    int cnt = ( l->depth > 0 ) ? 1 : 0; 
    for( int i = 0; i < 26; i++)
        if( l->child[i] != NULL )
            cnt += Letter_countSubstrings( l->child[i] , i, depth + 1); 

    return cnt; 
};

Letter Letter_buildTree( Letter head, enum side_t side, char * string ){

    Letter_increment( head, side );
    for ( int i = strlen( string ) - 1; i >= 0; i-- ){
        Letter_addSubstring( head, side, string + i );
    }
};
   
char * bufa[2056], bufb[2056];

int main( int agrc, char* argp, char* envp ) { 

    fflush(NULL);

    scanf( "%s", bufa); 
    scanf( "%s", bufb); 

    if( strlen(bufa) < 1 || strlen(bufb) < 1 ) {
        fprintf(stderr, "Improper inputs, exiting\n");
        exit(1); 
    }

    Letter head = Letter_new(0);
    Letter_buildTree( head, LEFT, bufa );
    Letter_buildTree( head, RIGHT, bufb );
    int c = Letter_countSubstrings( head, 0, 0 );

    printf( "%d\n", c );

  
}
#endif

