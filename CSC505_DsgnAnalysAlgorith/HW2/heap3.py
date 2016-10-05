from __future__ import division
from timer import timeFunction, function_data, report
import sys

class Heap3 :

  """
  Convienent get and set pattern
  """
  def __init__( i ) :         i.data = []
  def __getitem__( i, x ) :   return i.data[x-1]
  def __setitem__( i, x, y ): i.data[x-1] = y
  def __len__( i ) :          return len(i.data)
 
  """
  move around the tree
  """
  def left(i,k)   : return (k*3) - 1
  def center(i,k) : return (k*3)
  def right(i,k)  : return (k*3) + 1
  def up(i, k)    : return int(round( k / 3 ))

  """
  Heap functions
  """
  def heapify(i, j ):

    def getVal( item ) :
      if ( item > len( i ) ):
        return sys.maxint
      return i[item]
        
    m = min( j, i.left(j), i.center(j), i.right(j), key=getVal )
 
    if( m != j ) :
      i[m], i[j] = i[j], i[m]
      i.heapify( m )
     
  def increaseKey( i, key, val ) :
     if( i[key] > val ):
       raise ValueError( "val smaller than key")

     i[key] = val

     c = key
     while( 1 ) : 
       p = i.up( c )

       if( p == 0 ) : break

       if( i[p] > i[c] ) :
         i[p], i[c] = i[c], i[p]
         c = p
       else :
         break

  @timeFunction
  def insert( i, x ) :
    i.data.append(x)
    i.increaseKey( len(i), x )

  @timeFunction
  def remove( i, key=1 ):

    if( len(i) == 0 ) : return None 

    val = i.data.pop()

    if( len(i) == 0 ) : return val 

    ret = i[key]
    i[key] = val
    i.heapify( key )
    return ret

  def __str__( i ) : 
    sss = "HEAP : "
    row = 1
    cnt = 1
    for x in i.data  :
      sss += str(x) +" "
      cnt -= 1
      if( cnt == 0 ) :
        sss +="\nHEAP : "
        cnt = 3**row
        row += 1 
    return sss

    

if __name__ == '__main__' : 
   h = Heap3()
   for x in sys.stdin : 
     if( x.startswith( "add" ) ) : 
       h.insert( int(x[3:]) )
     elif( x.startswith( "remove" ) ):
       h.remove()
   report()


