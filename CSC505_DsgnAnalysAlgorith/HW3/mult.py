from __future__ import print_function
import sys 

inpt = [ int(x) for x in sys.stdin.readlines() if len(x.strip()) > 0]

n    = inpt[0]
dims = inpt[1:]
m    = [ [-1] * n for _ in range(n) ]
s    = [ [-1] * n for _ in range(n) ]

for i in range(n) : m[i][i] = 0

for length in xrange( 2, n+1 ) :

  for i in xrange( 0, n - length + 1 ): 

    j = i + length - 1
    m[i][j] = sys.maxint

    for k in xrange( i, j ):
      
      cost = m[i][k] + m[k+1][j] + dims[i] * dims[k+1] * dims[j+1]
   
      if ( cost < m[i][j] ):

        m[i][j] = cost
        s[i][j] = k

def split( path, s, e ) :

  def _split(s, e) :
    if s == e : 
      return "M"+str(s+1)
    else :
      k = path[s][e]
      return "( " + _split( s, k) + " * " + _split( k+1, e ) + " )" 
 
  k = path[s][e]
  return _split(s,k) + " * " + _split( k+1, e )

print( split( s, 0, n-1 ) )
