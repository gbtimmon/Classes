from __future__ import print_function
import random
import sys
import time

def getTime() : 
  return int( round( time.time() * 1000 ) )

def bsort(a, l=None, r=None):

  def s( a, l, r ):
    a[l], a[r] = a[r], a[l]   

  if l is None : l = 0 
  if r is None : r = len(a) - 1

  for x in range(r) :
    for y in range( r - x ) :
      if( a[y] > a[y+1] ) : s(a,y,y+1)

  return a

def qsort(a, l=None, r=None, k=0):

  def s( a, l, r ):
    a[l], a[r] = a[r], a[l]   

  def pivot(a, l, r):
    pidx = random.randint(l,r)
    p = a[pidx]
    s(a, r, pidx)

    lpart = l
    for idx in range(l, r):
      if a[idx] < p:
        s(a, idx, lpart)
        lpart += 1
        
    s(a, r, lpart)
    return lpart
    
  if( l is None ) : l = 0
  if( r is None ) : r = len(a) - 1

  if r > l:
    p = pivot(a, l, r)
    if( p - l <= k ) :
      bsort( a, l, p)
    else :
      qsort(a, l, p)

    if( r-p+1 <= k ):
      bsort(a,l,p)
    else:
      qsort(a, p+1, r)
  return a

if __name__ == '__main__' : 
  arr = []
  for x in sys.stdin :
    arr.append( int( x ) )

  t0 = getTime()
  qsort( arr ) 
  t1 = getTime()

  sys.stderr.write( "%d\n" %( t1 -t0 ) )

  
  print( *arr, sep="\n" )
