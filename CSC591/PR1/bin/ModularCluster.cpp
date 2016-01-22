#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <vector>
#include <list>

#include <./Cluster.cpp>

#define TRUE 1
#define FALSE 0 

int n; 
int m;
std::vector<Cluster> c; 
std::list<Cluster> c_l;

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
   /* ios for the file read                         */
   if ((fdin = open (argv[1], O_RDONLY)) < 0)
      fprintf (stderr,"can't open %s for reading", argv[1]);
   if (fstat (fdin,&statbuf) < 0)
      perror ("fstat error");
   if ((src = (char*) mmap (0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdin, 0)) == (caddr_t) -1)
      perror ("mmap error for input");
    
 
   char* token_start;
   bool in_token = false; 
   int first_vertex;
   int second_vertex;
   int tokens_seen = 0; 

   while(*src){
      if( in_token && !isdigit(*src) )
      {
         (*src) = '\0';
         in_token = false; 
         int token = atoi(token_start); 
         tokens_seen++;
         if ( tokens_seen == 1 ){
            n = token;   
            c.reserve(n); 
            for(int i = 0; i < n; i ++) { 
               c.push_back(Cluster(i));              
            }  
         } else if ( tokens_seen == 2) { 
            m = token;
         } else if ( tokens_seen % 2 == 1 ) {
            first_vertex = token;
         } else { //if (tokers+seen % 2 == 0 ) { 
            second_vertex = token;
            c[first_vertex].connectCluster(&c[second_vertex]);
            c[second_vertex].connectCluster(&c[first_vertex]);
         }
   
      } else if ( ! in_token && isdigit(*src) ) 
      {
         token_start = src; 
         in_token=true; 
      }
      src++;
   }
   printf("N=%d\nM=%d\n", n, m);
   printf("DS Contructed\n");

   //Aggregate single neoghbor vertexes
   for(int j = 0; j < c.size(); j++){
      if(c[j]){
          :q!

      }
   }
   

}

