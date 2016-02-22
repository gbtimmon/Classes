/******************************************************************************
 *
 *  File Name........: main.c
 *
 *  Description......: Simple driver program for ush's parser
 *
 *  Author...........: Vincent W. Freeh
 *
 *****************************************************************************/

#define _GNU_SOURCE

#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <parse.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_MODE 	       S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH 
#define DEFAULT_BUFFER_SIZE    2048
#define DEFAULT_CONFIG_FILE    "/.ushrc"

#define prompt()               printf("armadillo%%"); fflush(stdout);
#define IS_PARENT(x)           ( x > 0 )
#define IS_CHILD(x)            ( x == 0 )

#define C_SUCCESS              1
#define C_PARSE_FAILURE        2
#define C_CHILD_FAILURE        3
#define C_END                  4

#define STR_EQ(x,y)            !strcmp(x,y)
#define B_BG                   "bg"
#define B_CD                   "cd"
#define B_ECHO                 "echo"
#define B_FG                   "fg"
#define B_JOBS                 "jobs"
#define B_KILL                 "kill"
#define B_LOGOUT               "logout"
#define B_NICE                 "nice"
#define B_PWD                  "pwd"
#define B_SETENV               "setenv"
#define B_UNSETENV             "unsetenv"
#define B_WHERE                "where"


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
    if ( !strcmp (c->args[0], "end") ){
        printf("END! \n");
        exit(0);
    }
}

int builtins(Cmd c){
    /*** BG ***
     * Not Implemented
     */
    if( STR_EQ( c->args[0] , B_BG ) ) {
        printf("built-in BG not implemented\n");
        return EXIT_SUCCESS;

    /*** CD ***
     *  changes the cwd of the process
     * to the path specified. If path is not
     * valid then no effect. 
     */ 
    } else if( STR_EQ( c->args[0] , B_CD ) ) {
        
        return EXIT_SUCCESS;

    /*** ECHO ***
     * prints all args to stdout, no std in, replaces
     * linux echo def. 
     */
    } else if( STR_EQ( c->args[0] , B_ECHO ) ) {
        for(int i = 1; c->args[i] != NULL; i++) 
            printf("%s ", c->args[i]);
        printf("\b\n");
        return EXIT_SUCCESS;

    /*** FG ***
     * not implemented
     */
    } else if( STR_EQ( c->args[0] , B_FG ) ) {
        printf( "built in %s\n", B_FG );
        return EXIT_SUCCESS;

    
    } else if( STR_EQ( c->args[0] , B_JOBS ) ) {
        printf( "built in %s\n", B_JOBS );
        return EXIT_SUCCESS;
    } else if( STR_EQ( c->args[0] , B_KILL ) ) {
        printf( "built in %s\n", B_KILL );
        return EXIT_SUCCESS;
    } else if( STR_EQ( c->args[0] , B_LOGOUT ) ) {
        printf( "built in %s\n", B_LOGOUT );
        return EXIT_SUCCESS;
    } else if( STR_EQ( c->args[0] , B_NICE ) ) {
        printf( "built in %s\n", B_NICE );
        return EXIT_SUCCESS;

    /*** PWD ***
     * prints the pwd of the system. 
     */
    } else if( STR_EQ( c->args[0] , B_PWD ) ) {
        
        printf( "%s\n", getcwd(NULL, 0) );
        return EXIT_SUCCESS;

    } else if( STR_EQ( c->args[0] , B_SETENV ) ) {
        printf( "built in %s\n", B_SETENV );
        return EXIT_SUCCESS;
    } else if( STR_EQ( c->args[0] , B_UNSETENV ) ) {
        printf( "built in %s\n", B_UNSETENV );
        return EXIT_SUCCESS;
    } else if( STR_EQ( c->args[0] , B_WHERE ) ) {
        printf( "built in %s\n", B_WHERE );
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/*
 * dup_file 
 *
 * Procedure to redirect an existing OS file to a std file 
 * channel. Only ok to be run in a child process. Will kill
 * child on failure. 
 */
void dup_file(const char* fname, int flags, int dest_fd) {
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

static int runCmd(Cmd c) { 

    if( ! c ) 
        return C_PARSE_FAILURE; 
    if( !strcmp( c->args[0], "end") )    
        return C_END; 
    
    pid_t pid = fork();

    if( IS_PARENT( pid ) ) { 

        int rc;
        if(c->next != NULL ) { 

           rc = runCmd(c->next);
           if( rc != C_SUCCESS ) { 
               kill(pid, SIGTERM);
               return rc;
           }
        } 

        int status; 
        waitpid(pid, &status, 0);

        return ( WEXITSTATUS(status) == EXIT_SUCCESS ) 
            ? C_SUCCESS 
            : C_CHILD_FAILURE ;
           
           
         
    } else if ( IS_CHILD( pid ) ) {

        int rc = builtins(c);   
        if ( rc == EXIT_SUCCESS ) 
           return EXIT_SUCCESS;
        
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

const char* getHome() { 
    return (getenv("HOME") == NULL)
        ? getpwuid( getuid() )->pw_dir
        : getenv("HOME"); 
}

/*
 * Start the shell
 * init steps are 
 *  1.) try to open the .ushrc at $HOME
 *  2.) execute the config file
 */

int init() { 
    char file_path [DEFAULT_BUFFER_SIZE];
    strcpy(file_path, getHome());
    strcat(file_path, DEFAULT_CONFIG_FILE);
    
    int fd = open(file_path, O_RDONLY );

    if( fd < 0 ) { 
        fprintf(stderr, "Failed to open %s, conitnuing with ush\n", DEFAULT_CONFIG_FILE);
        return EXIT_FAILURE; 
    }
    
    int old_stdin = dup(STDIN_FILENO);
    dup_file( file_path, O_RDONLY, STDIN_FILENO ); 
    while( 1 ){
        Pipe p = parse();
        while( p != NULL ){
            int rc = runCmd(p->head);
            if( rc != EXIT_SUCCESS ) {
                dup2( old_stdin, STDIN_FILENO);
                return EXIT_SUCCESS;
            } 
            p = p->next; 
         }
    }
}

/*
 *mainLoop, where the magic happens 
 */
int mainLoop() {
    while(1){
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

int main(int argc, char *argv[])
{
    printf("DEBUG> %s\n", getHome());
    int rc; 
    rc = init();
    //if (rc == ERR) 
    rc = mainLoop();
    //if (rc == ERR)
}
