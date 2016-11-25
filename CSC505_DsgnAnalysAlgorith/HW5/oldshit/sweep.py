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

itr = 0 
while( heap ) : 

  itr += 1 
  print(("\n" + ("="*15) + "ITER %2d"+("="*15))%(itr))
   
  event = heap.pop()
  print( event ) 
  #heap.printHeap(n=5) 

  if( event.type == Type.L ) :
    this  = event.seg1
    tree.put( this.l, this.l, this ) 
    above = tree.next( this.l ) 
    below = tree.prev( this.l ) 

    print( "SEGS" )
    print( this )
    print( above )
    print( below ) 

    if above and below : 
      a = above.intersect( below ) 
      if a : 
        heap.remove( a )

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
    s = event.seg1
    t = event.seg2
    p = Point(event.x, event.y)

    print( "INTERSECT :", s, "and", t, "at",  p ) 

    tree.deleteByKey( s.l )
    tree.deleteByKey( t.l ) 

    a = Point( event.x, event.y + Point.eps ) 
    b = Point( event.x, event.y - Point.eps ) 

    tree.put( s.l, a, s ) 
    tree.put( t.l, b, t )

    r = tree.prev( s.l ) 
    u = tree.next( t.l )

    print("INTERSECT")
    print( r ) 
    print( s ) 
    print( t ) 
    print( u ) 

    if r :
      a = r.intersect(s)
      if a : 
        heap.remove( a ) 
      a = r.intersect(t)
      if a:
        heap.push( Event( a.x, a.y, Type.I, r, t ) ) 

    if u : 
      a = u.intersect( t ) 
      if a :
        heap.remove( a ) 

      a = u.intersect( s ) 
      if a : 
        heap.push( Event( a.x, a.y, Type.I, s, u ) ) 

  #print( [ "%s:%s"%(k,v.id) for k, v in tree.lkp.iteritems()] ) 
  #tree.printTree()
    

