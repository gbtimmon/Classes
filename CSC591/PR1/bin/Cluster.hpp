#ifndef _Cluster_hpp_
#define _Cluster_hpp_
#include <stdio.h>
#include <vector>

class Edge {
  int v1, v2, w; 

  public: 
     Edge(); 
     Edge(int, int, int); 
     void setWeight(int); 
     void toString();
     int  getWeight(); 
     void print(); 
};

class Cluster { 
  int w_internal;
  int w_external;
  std::vector<Edge> e;

  public : 
     Cluster(); 
     void addEdge(Edge); 
};
#endif
