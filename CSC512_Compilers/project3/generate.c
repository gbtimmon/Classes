#ifndef _GENERATE_C_
#define _GENERATE_C_

#include <stdio.h>
#include "generate.h"
#include "symbol.h"

int LABEL_COUNT = 0; 
FILE * GLOBAL_FILE; 

const char * tab_str ="    ";
const char * tab_str_cheat ="   "; // OMG What a hack. See what happens when I code for 10 hours straight?!.

void tab( int depth ) {
   for( int i = 0; i < depth; i++ ) fprintf(GLOBAL_FILE, tab_str);       
};

void tabCheat( int depth ){
   for( int i = 0; i < depth-1; i++ ) fprintf(GLOBAL_FILE, tab_str);       
   if( depth > 0 ) fprintf(GLOBAL_FILE,  tab_str_cheat ); 
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
     
char * emptyStr() { 
    char * e = malloc( sizeof(char) * 2 ); 
    sprintf( e, "" ); 
    return e; 
}

Token  getOffset( Token t ) {
    if( t->peer == NULL ) return NULL;
    if( t->peer->type == S_XOPER ) return t->peer; 
    if( t->peer->type == S_BRACK_EXP ) return t->peer->child; 
    return NULL;
};

char * getReference( SymbolTable global, SymbolTable local,  Token c, int depth ) { 

    int isLocal = 0; 

    Symbol sym = SymbolTable_find( local, c->value ); 

    if( sym == NULL ) {
        sym = SymbolTable_find( global, c->value ); 
        isLocal = 1; 
    }

    if( sym == NULL ) { 
        fprintf(stderr, "Compiler error, underfined reference to variable to %s\n", c->value ); 
        exit(1); 
    }

    Token offset = getOffset( c ); 
    if( sym->ref == -1 ) {
        char * out = malloc( sizeof(char) * (strlen(c->value) + 1 ) ); 
        strcpy( out, c->value ); 
        return out; 
    }
    
    if( offset == NULL ) {
        char * out = malloc( sizeof( char ) * 20 ); 
        sprintf( out, "%s[%d]", (isLocal==0) ? "local" : "global", sym->ref ); 
        return out;
    }
      
    if( offset->type == T_NUMBER ) { 
        int offnum = atoi( offset->value ); 
        char * out = malloc( sizeof( char ) * 20 ); 
        sprintf( out, "%s[%d]", (isLocal==0) ? "local" : "global", sym->ref + offnum); 
        return out;
    } 

    if( offset->type == T_VAR ) {
        char * ref = getReference( global, local, offset , depth); 
        char * out = malloc( sizeof( char ) * (20 + strlen(ref)) ); 
        sprintf(out, "%s[%d + %s]", (isLocal==0) ? "local" : "global", sym->ref, ref ); 
        free(ref); 
        return out;
    }
       
    if( offset->type == S_XOPER ) {
        int ref = putOperChain( global, local, offset, depth ); 
        char * out = malloc( sizeof( char ) * 27 ); 
        sprintf(out, "%s[%d + local[%d]]", (isLocal==0) ? "local" : "global", sym->ref, 7 ); 
        return out;
    }
    return emptyStr();
}

int putOperChain(SymbolTable global, SymbolTable local, Token t, int depth ) {

    int ref = SymbolTable_getTemp( local ); 

    char * out = malloc( sizeof( char ) * 25 );   
    sprintf(out, "local[%d] =", ref ); 
    
    Token c = t->child; 
    while ( c != NULL ) {
        if( c->type == S_XOPER ) {
                      
            int newref = putOperChain( global, local, c, depth); 
            char * newrefs[10]; 
            sprintf(  newrefs, " local[%d]", newref );
            dynamicStrcat( &out, newrefs ); 
        } else if ( c->type == S_BRACK_EXP ) {
            
        } else if ( c->type == T_VAR ) {
            char * refName = getReference( global, local, c, depth ); 
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
    fprintf(GLOBAL_FILE,  "%s;\n", out ); 
    free(out); 
    return ref; 
};

void putLiteral( Token c ) { 
     fprintf(GLOBAL_FILE,  "%s ", c->value); 
}

char * putStatement( SymbolTable global, SymbolTable local, Token t, int depth, int continuel, int breakl ){
    Token c = t->child; 

    int buf  = 100; 
    int used = 0; 
    char * out = malloc( sizeof( char ) * buf ); 
    sprintf(out, "" ); 

    while( c != NULL ){
        if( c->type == T_VAR ){ 
            char * name = getReference( global, local, c, depth ); 
            dynamicStrcat( &out, " " );
            dynamicStrcat( &out, name );
            free( name );
        } else if( c->type == S_BRACK_EXP ) {
  
        } else if( isTerminal(c->type) ) {
            dynamicStrcat( &out, " " );
            dynamicStrcat( &out, c->value );

        } else if( c->type == S_XOPER ) { 
       
            int ref = putOperChain( global, local, c, depth ); 
            char * temp [20];          
            sprintf(  temp, " local[%d]", ref); 
            dynamicStrcat( &out, temp ); 
        } 
        c= c->peer; 
    }
    tabCheat(depth); 
    fprintf(GLOBAL_FILE, "%s;\n", out);
    free( out ); 
}

char * putCondExp( SymbolTable global, SymbolTable local,  Token t , int depth ) {
    char * out = malloc( sizeof( char ) * 2 ) ; 
    sprintf(out, ""); 

    Token c = t->child; 
    while( c != NULL ) {
        dynamicStrcat(&out, " " );
        switch( c->type ) {
          case T_VAR :{
            char * ref = getReference( global, local, c, depth ); 
            dynamicStrcat( &out, ref ); 
            free(ref); 
            break; 
          }
          case T_NUMBER :
          case T_BOOL_OP :
          case T_CMP :
            dynamicStrcat( &out, c->value ); 
            break;
          case S_XOPER:{
            int ref = putOperChain( global, local, c, depth ); 
            char * str[20]; 
            sprintf( str, "local[%d]", ref ); 
            dynamicStrcat(&out, str ); 
            break;
          }
        }
        c = c -> peer; 
    }

    return out; 
}

void putWhileStatement( SymbolTable global, SymbolTable local, Token t, int depth  ) { 

     int looplbl = LABEL_COUNT++; 
     int exitlbl = LABEL_COUNT++; 
     int okaylbl = LABEL_COUNT++; 

     
     Token exp  = Token_findChild( t, S_COND_EXP ); 
     Token cnd  = exp->child; 

     tab( depth ); 
     fprintf(GLOBAL_FILE,  "label_%d: ;\n", looplbl ); 

     char * out = malloc( sizeof(char) * 100 ); 
     sprintf(  out, "if(" ); 

     char * condExp = putCondExp( global, local, exp, depth ); 
     dynamicStrcat( &out, condExp ); 
     free( condExp ); 

     tab(depth); 
     fprintf(GLOBAL_FILE,  out ); 
     free( out ); 
    
     fprintf(GLOBAL_FILE,  ") goto label_%d;\n", okaylbl ); 

     tab(depth);
     fprintf(GLOBAL_FILE, "goto label_%d;\n", exitlbl ); 
     
     tab(depth); 
     fprintf(GLOBAL_FILE, "label_%d: ;\n", okaylbl ); 

     Token statements = Token_findChild( t, S_BLOCK );
     writeStatements( global, local, statements, depth + 1, looplbl, exitlbl );

     tab(depth);   
     fprintf(GLOBAL_FILE, "goto label_%d;\n", looplbl); 

     tab(depth);   
     fprintf(GLOBAL_FILE, "label_%d: ;\n", exitlbl); 

}
void putIfStatement( SymbolTable global, SymbolTable local, Token t, int depth, int continuel, int breakl ) { 

     
     Token exp  = Token_findChild( t, S_COND_EXP ); 
     Token cnd  = exp->child; 

     char * out = malloc( sizeof(char) * 5 ); 
     sprintf(  out, "if(" ); 
     char * condExp = putCondExp( global, local, exp, depth ); 
     dynamicStrcat( &out, condExp ); 
     free( condExp ); 
     tab(depth);    
     fprintf(GLOBAL_FILE,  out ); 
     fprintf(GLOBAL_FILE,  ") ");
     free(out); 

     int label1 = LABEL_COUNT++; 
     int label2 = LABEL_COUNT++; 

     fprintf(GLOBAL_FILE, " goto label_%d;\n", label1); 
     tab(depth);   
     fprintf(GLOBAL_FILE, "goto label_%d;\n", label2);
     tab(depth);   
     fprintf(GLOBAL_FILE, "label_%d: ;\n", label1); 

     Token statements = Token_findChild( t, S_BLOCK );
     writeStatements( global, local, statements, depth + 1, continuel, breakl );

     tab(depth);   
     fprintf(GLOBAL_FILE, "label_%d: ;\n", label2); 
}

void writeStatements( SymbolTable global, SymbolTable local, Token t, int depth, int continuel, int breakl ) { 
    Token c = t->child; 
    while( c != NULL ) {
        if( c->child->type == T_IF ) {
            putIfStatement( global, local, c, depth, continuel, breakl ); 
        } else if( c->child->type == T_WHILE ) {
            putWhileStatement( global, local, c, depth ); 
        } else if ( c->child->type == T_CONTINUE ){
            if( continuel == -1 ) {
               fprintf(stderr, "Compile error : continue statement outside of block\n");
               exit(1); 
            }
            tab(depth); 
            fprintf(GLOBAL_FILE, "goto label_%d;\n", continuel);
        } else if ( c->child->type == T_BREAK ){
            if( continuel == -1 )
               fprintf( stderr, "break statement outside of block\n");
            tab(depth); 
            fprintf(GLOBAL_FILE, "goto label_%d;\n", breakl);
        } else {
            putStatement( global, local, c, depth, continuel, breakl ); 
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
    fprintf(GLOBAL_FILE, " %s %s", c->value, c->peer->value ); 
    
    c = c->peer->peer; 

    while( c != NULL ) {
        fprintf(GLOBAL_FILE, ", %s %s", c->value, c->peer->value ); 
        c = c->peer->peer; 
    }
    fprintf(GLOBAL_FILE, " "); 

};

void loadParms( SymbolTable local, Token t, int depth ) {

    Token c = t->child; 
    while( c != NULL ) { 
    
        if( c->type == T_VAR ) {
         
            Symbol sym = SymbolTable_find( local, c->value ); 
    
            if( sym != NULL ) {
                tab(depth);
                fprintf(GLOBAL_FILE,  "local[%d] = %s;\n", sym->ref, c->value); 
            }
        }

        c = c->peer; 
    }
    fprintf(GLOBAL_FILE,  "\n"); 
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

   fprintf(GLOBAL_FILE,  "int %s (", name->value ); 
   putParms( parm ); 
   fprintf(GLOBAL_FILE,  ")" );

   if( stat == NULL )
      fprintf(GLOBAL_FILE,  ";\n\n"); 
   else {
      fprintf(GLOBAL_FILE,  "{\n\n"); 
      if( local_count > 0 ) fprintf(GLOBAL_FILE, "    int local[%d];\n\n", local_count ); 
      if( parm != NULL ) loadParms( local, parm, 1 ); 
      writeStatements( global, local, stat, 1, -1,-1 ); 
      fprintf(GLOBAL_FILE, "};\n"); 
   }
};

void generate( FILE * f, Token meta, Token t ) {

    GLOBAL_FILE = f; 
    Token m = meta->child ; 

    while( m != NULL ){
        if( m->value != NULL ) 
            fprintf(GLOBAL_FILE,  "%s\n", m->value ); 
        m=m->peer; 
    }

    int global_size = SymbolTable_getAllocSize( t->scope ); 

    if( global_size > 0 )
        fprintf(GLOBAL_FILE,  "int global[%d];\n", global_size ); 
   
    Token c = t->child; 
    while( c != NULL ) {
        if( c->type == S_XFUNC )
            writeFunc( f, c ); 
        c = c->peer; 
    };
};

#endif 
