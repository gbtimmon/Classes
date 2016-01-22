#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0 

int n; 
int m;
int main (int argc, char *argv[])
{
  int fdin;
  char *src;
  struct stat statbuf;

  if (argc != 2){
    perror ("usage: a.out <fromfile>");
    exit(1);
  } 


  /* mmap the file to reduce the number of */
  /* ios for the file read                 */
  if ((fdin = open (argv[1], O_RDONLY)) < 0)
    fprintf (stderr,"can't open %s for reading", argv[1]);
  if (fstat (fdin,&statbuf) < 0)
    perror ("fstat error");
  if ((src = mmap (0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdin, 0)) == (caddr_t) -1)
    perror ("mmap error for input");
   
 
  char* token_start;
  bool in_token = false; 
  int tokens_seen = 0; 
  int* tokens; 
  while(*src){
    if( in_token && !isdigit(*src) )
    {
      (*src) = '\0';
      in_token = false; 
      int token = atoi(token_start); 
      if ( tokens_seen == 0 ){
        n = token;  
        tokens_seen++;
      } else if ( tokens_seen == 1) { 
        m = token;
        tokens_seen++; 
        tokens = (int*) malloc(sizeof(int) * m * 2 );
      } else {
        tokens[tokens_seen - 2] = token;
        tokens_seen++;       
      }
  
    } else if ( ! in_token && isdigit(*src) ) 
    {
      token_start = src; 
      in_token=true; 
    }
    src++;
  }
  printf("N=%d\nM=%d\n", n, m);
  for(int i = 0; i < 2*m; i=i+2){
      printf("EDGE : %d <-> %d\n", tokens[i] ,  tokens[i+1]);
  }
}

