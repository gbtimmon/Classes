/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: Simple driver program for ush's parser
 *
 *  Author...........: Vincent W. Freeh
 *
 *****************************************************************************/

#define _POSIX_SOURCE

#include <error.h>
#include <fcntl.h>
#include <parse.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_MODE 	     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH 
#define prompt()             printf("armadillo%%");
#define IS_PARENT(x)         ( x > 0 )
#define IS_CHILD(x)          ( x == 0 )


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

      printf("[");
      for ( i = 1; c->args[i] != NULL; i++ )
	printf("%d:%s,", i, c->args[i]);
      printf("\b]");
    }
    putchar('\n');
    // this driver understands one command
    if ( !strcmp(c->args[0], "end") ) exit(0);
}

/*
 * dup_file 
 *
 * Procedure to redirect an existing OS file to a std file 
 * channel. Only ok to be run in a child process. Will kill
 * child on failure. 
 */
void dup_file(const char* fname, int flags, int dest_fd) {
    static int errno = 0; 
    int fd = open(fname, flags, DEFAULT_MODE);
    if( fd < 0 ){
        fprintf(stderr, "[ush] Failed to open file %s, errno %d\n [%s]\n", fname, fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if ( dup2(fd, dest_fd) < 0 ) { 
        fprintf(stderr, "Failed to redirect %s to %d\n", fname, dest_fd);
        exit(EXIT_FAILURE);
    }
}

#define exists(x) ( x != Tnil)
static int runCmd(Cmd c) { 
   
    if( ! c ) return 0;

    pid_t pid = fork();

    if( IS_PARENT( pid ) ) { 

        int rc;
        if(c->next != NULL ) { 

           rc = runCmd(c->next);
           if( rc != EXIT_SUCCESS ) { 
               kill(pid, SIGTERM);
               return rc;
           }
        } 

        int status; 
        waitpid(pid, &status, 0);
        return  WEXITSTATUS(status);
           
         
    } else if ( IS_CHILD( pid ) ) {

        if( c->in == Tin ) 
            dup_file( c->infile, 0, STDIN_FILENO ); 

        if( c->out != Tnil ) { 
            switch( c->out ) {
                case Tout :    // >
                    dup_file(c->outfile, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO );
                    break;
                case Tapp :   // >>
                    dup_file(c->outfile, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO ) ;
                    break;
                case ToutErr: // >&
                    dup_file(c->outfile, O_WRONLY | O_CREAT , STDERR_FILENO );
                    break;
                case TappErr: // >>&
                    dup_file(c->outfile, O_WRONLY | O_CREAT | O_APPEND, STDERR_FILENO ) ;
                    break;
                case Tpipe:
                    fprintf(stderr, " | not yet implemented\n");
                    break;
                case TpipeErr : 
                    fprintf(stderr, " |& not yet implemented\n");
                    break;
                default : 
                    fprintf(stderr, "Shouldnt be here 4434\n");
                    exit(EXIT_FAILURE);
            }
        } 
        execvp( c->args[0], c->args );
        exit(EXIT_FAILURE);
    } else {  
        perror("Failed to fork, Fatal error\n");
        exit(1); 
    }
} 

int main(int argc, char *argv[])
{
    mode_t m = umask( (mode_t) 0 );
    printf("Umask : %o\n", (int) m );

    while ( 42 ) { 
        
        prompt();
        Pipe p = parse(); 
        
        while ( p != NULL ) { 

            prCmd(p->head);
            runCmd(p->head);
            p = p->next;
        }
        freePipe(p);
    }
}


/*....................... end of main.c ....................................*/
