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
std::vector<Cluster*> _c;
std::list<Cluster*>c;
std::list<Cluster*> completed_nodes;

int main (int argc, char *argv[]){

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
   
   list<Cluster*> comp;
   while(*src){
      if( in_token && !isdigit(*src) )
      {
         (*src) = '\0';
         in_token = false; 
         int token = atoi(token_start); 
         tokens_seen++;
         if ( tokens_seen == 1 ){
            n = token;   
            _c.reserve(n); 
            for(int i = 0; i < n; i ++) { 
               _c.push_back(new Cluster(i));              
            }  
         } else if ( tokens_seen == 2) { 
            m = 2 * token;
         } else if ( tokens_seen % 2 == 1 ) {
            first_vertex = token;
         } else { //if (tokers+seen % 2 == 0 ) { 
            second_vertex = token;
            _c[first_vertex]->connectCluster(_c[second_vertex]);
            _c[second_vertex]->connectCluster(_c[first_vertex]);
         }
   
      } else if ( ! in_token && isdigit(*src) ) 
      {
         token_start = src; 
         in_token=true; 
      }
      src++;
   }
   int nodes_removed = 0;
   int lowest_degree_ind, lowest_degree;
   for(int i = 0; i < _c.size(); i++){
      c.push_back(_c[i]);
   }
   while(c.size() != 0){
      fprintf(stderr, "\r%d Reamining", c.size());
      /*********************************
 *        * Identify the single neighbors *
 *               *********************************/
      //printf("STEP1 CHECK FOR AND REMOVE SINGLE NEIGHBORS\n");
      for(auto i = c.begin(); i!=c.end();){
         if((*i)->tau() == 1){
            (*i)->e.begin()->first->absorbCluster(*i);
            i = c.erase(i);
         }else{
            i++;
         }
      }
      /******************************
 *        * Identify the lowest degree *
 *               * ****************************/
      //printf("STEP2 CHECK FOR LOWEST DEGREE VERTEX\n");
      lowest_degree = m+1;
      auto it = c.end();
      for(auto i = c.begin(); i!=c.end();i++){
          int deg = (*i)->getDegree();
          if( deg < lowest_degree){
               lowest_degree = deg;
               it = i;
          }
      }
      //printf("    lowest degree vertex %d with %d\n", lowest_degree_ind, lowest_degree);
      /**************************
 *        * Find the max Delta Mod *
 *               **************************/
      //printf("STEP3 get delta_mod for LDV\n");
      Cluster* absorber =(*it)->dModMax(m);
      if(absorber){
          absorber->absorbCluster(*it);
          c.erase(it);
          delete *it;
      } else {
          //completed_nodes.push_back(c[lowest_degree_ind]);
          comp.push_back(*it);
          c.erase(it);
      }
   }
   double mod = 0;
   for(auto i = comp.begin(); i != comp.end(); i++){
       (*i)->print();
       double fm = (double) m;
       double e_uu = (double) ((*i)->w_internal);
       printf("Euu %f\n", e_uu);
       double a_u  = (double) ((*i)->w_internal + (*i)->w_external);
       printf("a_u %f\n", a_u);
       double v1 = e_uu / fm;
       double v2 = (a_u * a_u) / ( fm * fm );
       mod += v1 - v2;
   }
   printf("Modulatiry %f\n", mod);
}

