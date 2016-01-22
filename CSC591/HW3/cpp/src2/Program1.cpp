#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp> 

using namespace std; 
struct PAIR {
  int key; 
  int cnt; 
  PAIR() :key(-1), cnt(1) {};
};

int n, m;
vector<string> loadfile(char*);
vector<PAIR> map( vector<string> );
vector<int> reduce ( vector <PAIR> );
void parseLine( string, int*, int*); 

int main(int argc, char* argv[]){ 
  if ( argc < 2) {
    cerr << "Arguments Please!\n";
    exit(1); 
  }

  vector<string> filev = loadfile( argv[1] );
  vector<PAIR> mapo = map( filev );
  vector<int> redo = reduce( mapo ); 

//  for( int i = 0; i < n; i++ ) { 
 //   cout << i << " " << redo[i] << "\n";
  //}
}

vector<string> loadfile( char *  filen ) { 
  ifstream myfile ( filen );
  string line; 
  getline( myfile, line);
  parseLine( line, &n, &m); 
  
  vector <string> output (m); 
  for(int i = 0; i < m; i++ ){
    getline( myfile, line);
    output[i] = line; 
  }
  return output;
}

vector<PAIR> map ( vector<string> in ) { 
  int v1, v2; 
  vector<PAIR> output ( 2*m, PAIR() );

  for(int i = 0; i < m; i++){
    parseLine( in[i].c_str() , &v1, &v2 );
    output[i  ].key = v1;
    output[i+m].key = v2;   
  }
  return output;
}

vector<int> reduce ( vector<PAIR> in ) { 
  vector<int> output (n, 0);
  for(int i = 0; i < 2*m; i++){
     output[ in[i].key ] += in[i].cnt;
  }
  return output;
}
void parseLine( string in, int* out1, int* out2 ) { 
   vector<string> toks; 
   boost::split(toks, in, boost::is_any_of("\t "));
   (*out1) = boost::lexical_cast<int>( toks[0] );
   (*out2) = boost::lexical_cast<int>( toks[0] );
} 
