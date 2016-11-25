from __future__ import print_function

def naive( haystack, needle ) : 
  for i in xrange( len( haystack ) - len( needle ) + 1):
    match = True
    for j in xrange( len( needle ) ):
      if haystack[i + j ] != needle[ j ] :
        match = False
        break
    if match : 
      return i
  return -1 

def KMP( T, P ) : 

  m = len(P)
  n = len(T)
  pi = [0] * m 
  k = -1 
  for i in xrange(1, m ) :
    while k > 0 and P[k+1] != P[i] : 
      k = pi[k+1] 

    if P[k+1] == P[i] : 
      k += 1 

    pi[i] = k 

  q=0
  for i in xrange( n ) :
    while q > 0 and P[q] != T[i]:
      q = pi[q]
    if P[q] == T[i] : 
      q += 1
    if q == m :
      return i - m + 1 
  return -1 

if __name__ =='__main__' : 

  import sys
  from time   import clock
  from random import choice, seed

  if len(sys.argv) > 1 : 
    f = open( sys.argv[1] ).readlines()
    needle   = f[1].strip()
    haystack = f[0].strip()
    print( needle) 
    print( haystack ) 
  else :  
    seed(100)
    needle   = "hey please dont start the car yet"
    haystack = " ".join([ choice(needle.split() ) for _ in range( 1000000 ) ])

  
  a = clock()
  i = naive( haystack, needle )
  b = clock()
  print( "found at:", i ) 
  print( "naive search time:", int( (b - a) * 1000) )

  a = clock()
  i = haystack.find(needle)
  b = clock()
  print( "found at:", i ) 
  print( "standard search time:", int( (b - a) * 1000 ) )

  a = clock()
  i = KMP( haystack, needle ) 
  b = clock()
  print( "found at:", i ) 
  print( "kmp search time:", int( ( b - a ) * 1000 ) )

  
