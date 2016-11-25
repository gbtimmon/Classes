from heapq       import heappush, heappop, heapify
from Point       import Point
from operator    import itemgetter

class Heap(object):
  def __init__(s) : s.data = []; 
  def push(s,e)   : heappush(s.data, e)
  def pop(s)      : return heappop(s.data) 
  def __len__(s)  : return len(s.data)
  def __iter__(s) : return s
  def remove(s,k) : 
    try : 
      i = next(( i for i, e in enumerate( s.data ) if Point( e.x, e.y) == k ) ) 
    except : 
      return 
    s.data[i] = s.data[-1]
    s.pop()
    heapify( s.data )
  def __str__( s ) :
    return str(s.data) 

  def printHeap( s, n=100 ) :
    data2 = s.data[:]

    print("HEAP CONTENTS")
   
    itr = 0 
    if data2 : 
      while( data2 and itr < n) :
        itr+=1
        print( heappop(data2 ) )
    else : 
      print("empty")
    
  
  
