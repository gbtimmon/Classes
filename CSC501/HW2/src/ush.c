/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: Simple driver program for ush's parser
 *
 *  Author...........: Vincent W. Freeh
 *
 *****************************************************************************/

#include <parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define prompt()    printf("armadillo%%");

static void prCmd(Cmd c)
{
  int i;

  if ( c ) {
    printf("%s%s ", c->exec == Tamp ? "BG " : "", c->args[0]);
    if ( c->in == Tin )
      printf("<(%s) ", c->infile);
    if ( c->out != Tnil )
      switch ( c->out ) {
      case Tout:
	printf(">(%s) ", c->outfile);
	break;
      case Tapp:
	printf(">>(%s) ", c->outfile);
	break;
      case ToutErr:
	printf(">&(%s) ", c->outfile);
	break;
      case TappErr:
	printf(">>&(%s) ", c->outfile);
	break;
      case Tpipe:
	printf("| ");
	break;
      case TpipeErr:
	printf("|& ");
	break;
      default:
	fprintf(stderr, "Shouldn't get here\n");
	exit(-1);
      }

    if ( c->nargs > 1 ) {
      printf("[");
      for ( i = 1; c->args[i] != NULL; i++ )
	printf("%d:%s,", i, c->args[i]);
      printf("\b]");
    }
    putchar('\n');
    // this driver understands one command
    if ( !strcmp(c->args[0], "end") )
      exit(0);
  }
}

#define exists(x) ( x != Tnil)
static void runCmd(Cmd c) { 
    if( !c ) return; 

    int    bg       = (c->exec == Tamp);
    char*  cmd      =  c->args[0];
    
} 

int main(int argc, char *argv[])
{
    while ( 42 ) { 
        int pn = 0; //DELME
        int cn = 0; //DELME

        prompt();

        Pipe p = parse(); 
        while ( p != NULL ) { 
            pn++;
            Cmd c = p->head; 
            while( c != NULL ) {
                 printf("[%d,%d]", pn, ++cn);
                 prCmd(c); 
                 runCmd(c);
                 c = c->next; 
            }
            p = p->next;
        }
        freePipe(p);
    }
}

/*........................ end of main.c ....................................*/
