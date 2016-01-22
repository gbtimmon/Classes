//#include <Cluster.hpp>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

class Cluster {
  public :
     vector<int>        nodes;
     int                w_internal;
     int                w_external;
     map<Cluster*, int> e;

     Cluster(int);
     void     absorbCluster(Cluster*);  
     void     addEdge(Cluster*, int);
     void     connectCluster(Cluster*);
     Cluster* dModMax(int);
     int      getDegree();
     void     dropEdge(Cluster*);
     int      getNodeId();
     void     print();
     void     printEdges();
     void     redirectEdge(Cluster*, Cluster*);
     int      tau();
};

Cluster::Cluster(int init_node){
  this->nodes.push_back(init_node);
  this->w_internal = 0; 
  this->w_external = 0; 
};

void Cluster::connectCluster(Cluster* dest){
   this->e.insert( pair<Cluster*, int>(dest, 1) );
   w_external++;
};

int Cluster::getNodeId(){
  return nodes[0]; 
};

void Cluster::dropEdge(Cluster* dest){
    map<Cluster*, int>::iterator ie;

    ie = this->e.find(dest);
    if( ie != this->e.end() ) {
        this->w_external -= ie->second;
        this->e.erase(ie);
    }

    ie = dest->e.find(this);
    if( ie != dest->e.end() ) {
        dest->w_external -= ie->second;
        dest->e.erase(ie);
    }
}
void Cluster::addEdge(Cluster* dest, int weight) {
   map<Cluster*, int>::iterator ie;
   
   //Add an edge from me to the dest. 
   ie = this->e.find( dest );
   this->w_external += weight;
   if( ie == this->e.end() ) {
      this->e.insert( pair<Cluster*,int>(dest,weight) );
   } else {
      ie->second += weight; 
   } 
  
   //Add an edge from dest to me.
   ie = dest->e.find( this ); 
   dest->w_external += weight;
   if( ie == dest->e.end() ) {
      dest->e.insert( pair<Cluster*,int>(this,weight) ); 
   } else {
      ie->second += weight;
   }
};

void Cluster::redirectEdge(Cluster* from, Cluster* to){
   map<Cluster*, int>::iterator ie = this->e.find(from);
   this->addEdge(to, ie->second); 
   this->dropEdge(from);
};

void Cluster::absorbCluster(Cluster* dest){
   //Flip points point to absorbee to the absorber.
   map<Cluster*, int>::iterator it;  
   for( it = dest->e.begin(); it != dest->e.end(); it++){
      it->first->redirectEdge(dest, this); 
   }
   //Absorb the dest internal loop.
   this->w_internal += dest->w_internal;

   //If making a new internal edge. 
   //Delete it and add weight X2 to internal
   it = this->e.find(this);
   if(it != this->e.end() ) {
      this->w_external -= ( it->second );
      this->w_internal += ( it->second );
      this->e.erase(it);
   }
   
   //Eat the nodes of the Dest
   this->nodes.insert(this->nodes.end(), dest->nodes.begin(), dest->nodes.end());
};
void Cluster::printEdges(){
    map<Cluster*, int>::iterator ie;
    for( ie = this->e.begin(); ie != this->e.end(); ie++){
        printf("   [%d -(%d)-> %d]\n", this->getNodeId(), ie->second, ie->first->getNodeId());
    }
};
void Cluster::print(){
    vector<int>::iterator in;
    printf("[");
    //for(in = this->nodes.begin(); in != this->nodes.end(); in++){
    //    printf("%d ", (*in));
    //}
    printf("] w_internal %d, w_external %d\n", this->w_internal, this->w_external);
};

int Cluster::tau(){
   return this->e.size();
};

Cluster* Cluster::dModMax(int m2){
    long maxMod = 0; 
    Cluster* max = NULL;
    if(! this->e.size() ) return NULL;
    for(auto it = this->e.begin(); it != this->e.end(); it++){
        long curMob = ( m2 * (it->second) ) - ( this->w_internal + this->w_external) * (it->first->w_internal + it->first->w_external);
        //printf("     Me (%d) to e(%d) mod = %d\n", this->getNodeId(), it->first->getNodeId(), curMob);
        if(maxMod < curMob){
            maxMod=curMob; 
            max=it->first;
        }
    }
    return max;
};

int Cluster::getDegree(){
   return this->w_internal + this->w_external;
};
