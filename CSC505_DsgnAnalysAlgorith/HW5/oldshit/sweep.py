from __future__  import print_function
from collections import namedtuple 
from enum        import Enum 

import sys 

from Segment import Segment
from Point   import Point
from Heap    import Heap 
from BinTree import BinTree

Event = namedtuple( 'Event', ['x', 'y', 'type', 'seg1', 'seg2' ] ) 
Type  = Enum( 'Type', "L R I" ) #LEFT RIGHT INTERSECT
heap  = Heap() 
tree  = BinTree()

for l in sys.stdin.readlines() : 
  arr = map( float, l.split() ) 

  if ( len( arr ) != 4 ) : continue 

  a = Point( arr[0], arr[1] ) 
  b = Point( arr[2], arr[3] ) 

  if( a.x > b.x ) : a,b = b,a

  s = Segment( a, b ) 

  heap.push(Event(s.l.x, s.l.y, Type.L, s, None ) ) 
  heap.push(Event(s.r.x, s.r.y, Type.R, s, None ) ) 

while( heap ) : 

  event = heap.pop()

  if( event.type == Type.L ) :
    this  = event.seg1
    tree.put( this.l, this.l, this ) 
    above = tree.next( this.l ) 
    below = tree.prev( this.l ) 

    if above :
      a = this.intersect( above ) 
      if a : 
        heap.push(Event( a.x, a.y, Type.I, this, above ) ) 

    if below : 
      a = this.intersect( below ) 
      if a:
        heap.push(Event( a.x, a.y, Type.I, below, this ) ) 
    
  if( event.type == Type.R ) : 

    this  = event.seg1
    above = tree.next( this.l ) 
    below = tree.prev( this.l )
    tree.deleteByKey( this.l ) 

    if above and below :
      a = above.intersect(below) 
      if a :
        heap.push( Event( a.x, a.y, Type.I, below, above ) )
    
  if( event.type == Type.I ) :
    print( "%3.2f %3.2f"%(round( event.x, 2), round(event.y, 2) ) )
    sys.exit(1)

    

