#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std; 
struct PAIR {
  int key; 
  int cnt; 
  PAIR() :key(-1), cnt(1) {};
};

long n, m;
vector<string> loadfile(char*);
vector<PAIR> map( vector<string> );
vector<int> reduce ( vector <PAIR> );
void parseLine( const char[], long*, long*); 

int main(int argc, char* argv[]){ 
  if ( argc < 2) {
    cerr << "Arguments Please!\n";
    exit(1); 
  }

  vector<string> filev = loadfile( argv[1] );
  vector<PAIR> mapo = map( filev );
  vector<int> redo = reduce( mapo ); 

  //for( int i = 0; i < n; i++ ) { 
  //  cout << i << " " << redo[i] << "\n";
  //}
}

vector<string> loadfile( char *  filen ) { 
  ifstream myfile ( filen );
  string line; 
  getline( myfile, line);
  parseLine( line.c_str(), &n, &m); 
  
  vector <string> output (m); 
  for(long i = 0; i < m; i++ ){
    getline( myfile, line);
    output[i] = line; 
  }
  return output;
}

vector<PAIR> map ( vector<string> in ) { 
  long v1, v2; 
  vector<PAIR> output ( 2*m, PAIR() );

  for(long i = 0; i < m; i++){
    parseLine( in[i].c_str() , &v1, &v2 );
    output[i  ].key = v1;
    output[i+m].key = v2;   
  }
  return output;
}

vector<int> reduce ( vector<PAIR> in ) { 
  vector<int> output (n, 0);
  for(long i = 0; i < 2*m; i++){
     output[ in[i].key ] += in[i].cnt;
  }
  return output;
}
void parseLine(const char in[], long* out1, long* out2 ) { 
   int index = 0;
   char t [10];
   int t_index = 0; 
   

   while( ! isdigit( in[index] ) ) { 
     index++;
   }
   while ( isdigit( in[index] ) ) {
     t[t_index] = in[index]; 
     index++;
     t_index++;
   }
   t[t_index] = '\0';
   (*out1) = atoi( t ); 
   t_index = 0;
   
   while( ! isdigit( in[index] ) ) {
     index++;
   }
   while ( isdigit( in[index] ) ) {
     t[t_index] = in[index];
     index++;
     t_index++;
   }
    t[t_index] = '\0';
   (*out2) = atoi( t );
} 
