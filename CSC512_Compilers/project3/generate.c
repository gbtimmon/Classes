#ifndef _GENERATE_C_
#define _GENERATE_C_

#include <stdio.h>
#include "generate.h"
#include "symbol.h"

int LABEL_COUNT = 0; 

void writeMeta( FILE * f, Token t ) { 
   Token c = t->child; 
   while( c != NULL ) {
       fprintf( f, "GenMeta : %s\n", c->value ); 
       c = c->peer; 
   }
};       

const char * tab_str ="    ";
const char * tab_str_cheat ="   "; // OMG What a hack. See what happens when I code for 10 hours straight?!.

void tab( int depth ) {
   for( int i = 0; i < depth; i++ ) printf(tab_str);       
};

void tabCheat( int depth ){
   for( int i = 0; i < depth-1; i++ ) printf(tab_str);       
   if( depth > 0 ) printf( tab_str_cheat ); 
}
   
//very lazy -- but perfroms well enough for quick coding. 
//Java esque style immutable. Dont judge me, Im running out of time ;). 
void dynamicStrcat( char ** dest, char * src ){
     int news = strlen( *dest ) + strlen( src ); 
  
     char * new = malloc( sizeof( char ) * (news + 2 ) ); 
     strcpy( new, *dest ); 
     strcat( new, src ); 

     free(*dest); 
     *dest = new; 
}
     


char * getReference( SymbolTable global, SymbolTable local,  Token c ) { 

    Symbol locS = SymbolTable_find( local, c->value ); 
    Symbol gloS = SymbolTable_find( global, c->value ); 


    char * ret = malloc( sizeof( char ) * 20 ); 

    if( locS != NULL ){
       int ref = locS->ref; 
       if( ref == -1 ) {
           sprintf(ret, "%s", c->value); 
       } else {
           sprintf(ret, "local[%d]", ref ); 
       }

    } else if ( gloS != NULL ) {
       int ref = gloS->ref; 
       if( ref == -1 ) {
           sprintf( ret, "%s", c->value); 
       } else {
           sprintf( ret, "global[%d]", ref ); 
       }
    } else {
       fprintf(stderr, "Undefined reference to variable %s.\n", c->value ); 
       //exit(1); 
    }
    return ret;
}

int putOperChain(SymbolTable global, SymbolTable local, Token t, int depth ) {

    int ref = SymbolTable_getTemp( local ); 

    char * out = malloc( sizeof( char ) * 25 );   
    sprintf( out, "local[%d] =", ref ); 
    
    Token c = t->child; 
    while ( c != NULL ) {
        if( c->type == S_XOPER ) {
                      
            int newref = putOperChain( global, local, c, depth); 
            char * newrefs[10]; 
            sprintf( newrefs, " local[%d]", newref );
            dynamicStrcat( &out, newrefs ); 

        } else if ( c->type == T_VAR ) {
            char * refName = getReference( global, local, c ); 
            dynamicStrcat( &out, " "); 
            dynamicStrcat( &out, refName); 
            free( refName );
        } else {
            dynamicStrcat( &out, " "); 
            dynamicStrcat( &out, c->value ); 
        }

        c = c->peer; 
    }   
    tab(depth);
    printf( "%s;\n", out ); 
    free(out); 
    return ref; 
};

void putLiteral( Token c ) { 
     printf( "%s ", c->value); 
}

char * putStatement( SymbolTable global, SymbolTable local, Token t, int depth ){
    Token c = t->child; 

    int buf  = 100; 
    int used = 0; 
    char * out = malloc( sizeof( char ) * buf ); 
    sprintf( out, "" ); 

    while( c != NULL ){
        if( c->type == T_VAR ){ 
            char * name = getReference( global, local, c ); 
            dynamicStrcat( &out, " " );
            dynamicStrcat( &out, name );
            free( name );

        } else if( isTerminal(c->type) ) {
            dynamicStrcat( &out, " " );
            dynamicStrcat( &out, c->value );

        } else if( c->type == S_XOPER ) { 
       
            int ref = putOperChain( global, local, c, depth ); 
            char * temp [20];          
            sprintf( temp, " local[%d]", ref); 
            dynamicStrcat( &out, temp ); 
        } 
        c= c->peer; 
    }
    tabCheat(depth); 
    printf("%s;\n", out);
    free( out ); 
}

void putIfStatement( SymbolTable global, SymbolTable local, Token t, int depth ) { 

     
     Token exp  = Token_findChild( t, S_COND_EXP ); 
     Token cnd  = exp->child; 

     int buf = 100; 
     char * out = malloc( sizeof(char) * buf ); 
     sprintf( out, "if(" ); 
     int used = strlen( out ); 

     while( cnd != NULL ) {
         if( cnd ->type == S_COND ){
             Token opr = cnd->child; 
             while( opr != NULL ) {
                 if( opr->type == S_XOPER ){
                     int ref = putOperChain( global, local, opr, depth ); 
                     char * temp[20];
                     sprintf(temp, "local[%d]", ref ); 
                     dynamicStrcat( &out, temp ); 
                 } else if ( opr->type == T_VAR ){
                     char * ref = getReference( global, local, opr ); 
                     dynamicStrcat( &out, ref ); 
                     free( ref ); 
                 } else {
                     dynamicStrcat( &out, opr->value ); 
                 } 
                 opr = opr->peer; 
             } 
         } else {
             dynamicStrcat(&out, " "); 
             dynamicStrcat(&out, cnd->value ); 
         }
         cnd = cnd -> peer; 
     }
            
     tab(depth);    
     printf( out ); 
     printf( ") ");
     free(out); 

     int label1 = LABEL_COUNT++; 
     int label2 = LABEL_COUNT++; 

     printf(" goto label_%d;\n", label1); 
     tab(depth);   
     printf("goto label_%d;\n", label2);
     tab(depth);   
     printf("label_%d: ;\n", label1); 

     Token statements = Token_findChild( t, S_BLOCK );
     writeStatements( global, local, statements, depth + 1 );

     tab(depth);   
     printf("label_%d: ;\n", label2); 
}

void writeStatements( SymbolTable global, SymbolTable local, Token t, int depth ) { 
    Token c = t->child; 
    while( c != NULL ) {
        if( c->child->type == T_IF ) {
            putIfStatement( global, local, c, depth ); 
        } else {
            putStatement( global, local, c, depth ); 
        }
        c=c->peer; 
    }
};
    

SymbolTable getLocalScope( Token t ){
    Token c = t; 
    while( c-> type != S_XFUNC ) c=c->parent; 
    return c->scope; 
}

SymbolTable getGlobalScope( Token t ){
    Token c = t; 
    while( c-> type != S_START ) c=c->parent; 
    return c->scope; 
}

void putParms( Token t ) {
    if( t == NULL || t->child == NULL || t->child->peer == NULL )
        return; 

    Token c = t->child;
    printf(" %s %s", c->value, c->peer->value ); 
    
    c = c->peer->peer; 

    while( c != NULL ) {
        printf(", %s %s", c->value, c->peer->value ); 
        c = c->peer->peer; 
    }
    printf(" "); 

};

void loadParms( SymbolTable local, Token t, int depth ) {

    Token c = t->child; 
    while( c != NULL ) { 
    
        if( c->type == T_VAR ) {
         
            Symbol sym = SymbolTable_find( local, c->value ); 
    
            if( sym != NULL ) {
                tab(depth);
                printf( "local[%d] = %s;\n", sym->ref, c->value); 
            }
        }

        c = c->peer; 
    }
    printf( "\n"); 
};

void writeFunc( FILE * f, Token t ) {

   SymbolTable local  = getLocalScope( t ); 
   SymbolTable global = getGlobalScope( t ); 

   Token type = Token_findChild( t, T_XTYPE ); 
   Token name = Token_findChild( t, T_VAR ); 
   Token parm = Token_findChild( t, S_PARM_LIST ); 
   Token data = Token_findChild( t, S_XDATA ); 
   Token stat = Token_findChild( t, S_XCODE ); 

   int local_count = SymbolTable_getAllocSize(local); 

   printf( "int %s (", name->value ); 
   putParms( parm ); 
   printf( ")" );

   if( stat == NULL )
      printf( ";\n\n"); 
   else {
      printf( "{\n\n"); 
      if( local_count > 0 ) printf("    int local[%d];\n\n", local_count ); 
      if( parm != NULL ) loadParms( local, parm, 1 ); 
      writeStatements( global, local, stat, 1 ); 
      printf("};\n"); 
   }
};

void generate( FILE * f, Token meta, Token t ) {

    Token m = meta->child ; 

    while( m != NULL ){
        if( m->value != NULL ) 
            printf( "%s\n", m->value ); 
        m=m->peer; 
    }

    int global_size = SymbolTable_getAllocSize( t->scope ); 

    if( global_size > 0 )
        printf( "int global[%d];\n", global_size ); 
   
    Token c = t->child; 
    while( c != NULL ) {
        if( c->type == S_XFUNC )
            writeFunc( f, c ); 
        c = c->peer; 
    };
};

#endif 
