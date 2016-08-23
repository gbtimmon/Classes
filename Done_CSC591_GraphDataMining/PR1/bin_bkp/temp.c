#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

FILE *input_graph; 
int n, m, c;
struct Cluster {
  int internal_edge_weight;
  int external_edge_weight; 
  int node_list[10000];
};

int main( int argc, const char* argv[] ){
  
  printf("Hello World\n");
  for( int i = 0; i < argc; i++ ) { 
    printf("input %d : %s\n", i, argv[i]);
  }
 
  if( argc < 2 ) {
    fprintf(stderr, "Expected a filepath at index 2\n");
  }

  int f_in; 
  char *src; 
  struct stat statbuf;
  
  if (( f_in = open ( argv[1] , O_RDONLY )) < 0){
    fprintf(stderr, "cant open file\n");
  }
 
  if (( src = mmap ( 0, 
}
