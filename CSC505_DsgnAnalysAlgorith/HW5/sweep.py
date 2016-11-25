from __future__  import print_function
from collections import namedtuple 
from enum        import Enum 
from heapq       import heappush, heappop, heapify
from operator    import itemgetter

import sys

class Point( object ) : 
  eps = 0.000000001
  def __init__( s, x, y ) : s.x, s.y = float(x), float(y)
  def __lt__( s, o )      : return Point.y_sorter( s, o ) == -1
  def __ge__( s, o )      : return Point.y_sorter( s, o ) >= 0
  def __sub__( s, o )     : return Point( s.x - o.x, s.y - o.y )
  def __add__( s, o )     : return Point( s.x + o.x, s.y + o.y )
  def scale( s, m )       : return Point( s.x * m, s.y * m )
  def perp( s )           : return Point( -s.y, s.x )
  def dot( s, o )         : return ( s.x * o.x ) + (  s.y * o.y )
  @staticmethod
  def cross( s, o ) : return (s.x * o.y) - ( o.x * s.y )
  @staticmethod 
  def x_eq( a, b ) : return abs( a.x - b.x ) < Point.eps 
  @staticmethod 
  def y_eq( a, b ) : return abs( a.y - b.y ) < Point.eps 
  
  @staticmethod
  def x_sorter( a, b ) :
    if Point.x_eq( a, b): 
      return 0
    if ( a.x < b.x )                : 
      return -1
    return  1

  @staticmethod
  def y_sorter( a, b ) :
    if Point.y_eq(a, b ) : 
      return Point.x_sorter(a,b) 
    if ( a.y < b.y ) :              
      return -1 
    return  1

class Segment ( object ) :

  def __init__( s, l, r ) : 
    s.l,s.r = l, r 

  def slope(s):
    if s.l.x == s.r.x : return None
    else: return float(s.r.y-s.l.y)/(s.r.x-s.l.x)

  def intersect( s, o ) :
    v2 = s.r - s.l 
    v4 = o.r - o.l
    t4 = float((s.l - o.l).dot( v2.perp() )) / float( v4.dot( v2.perp() ) + Point.eps )
    t2 = float((o.l - s.l).dot( v4.perp() )) / float( v2.dot( v4.perp() ) + Point.eps ) 
    I = o.l + ( v4.scale( t4 ) ) 
    if( not( 0.0 <= t2 <= 1.0 and 0.0 <= t4 <= 1.0 ) ) : 
      return None 
    return I 

class BinTree(object) :

  class Node(object) :
    def __init__( s, key, seg, l, r, p ):
      s.key    = key
      s.seg    = seg
      s.left   = l
      s.right  = r
      s.parent = p

    def pri( s, l ) :
      m = s.seg.slope() 
      p = s.seg.l
      return Point( l.x, p.y + ( ( l.x - p.x) * m ))

  def __init__(s) :
    s.root = None
    s.size = 0
    s.lkp  = {}

  def __len__(s) :
    return s.size

  def put( s, key, at, seg ):
    s.size += 1
    if s.root == None :
      newNode    = BinTree.Node( key, seg, None, None, None )
      s.root     = newNode
      s.lkp[key] = newNode
      return

    cur = s.root
    while True :
      if cur.pri(at) >= key:
        if cur.left :
          cur = cur.left
        else :
          newNode    = BinTree.Node( key, seg, None, None, cur )
          cur.left   = newNode
          s.lkp[key] = newNode
          return
      else :
        if cur.right :
          cur = cur.right
        else :
          newNode    = BinTree.Node( key, seg, None, None, cur )
          cur.right  = newNode
          s.lkp[key] = newNode
          return

  def getByKey( s, key ) :
    return s.lkp[key]

  def deleteByKey( s, key ) :
    n = s.getByKey( key )
    s.remove( n )
    s.size -= 1

  def removeLeaf( s, n ) : 
    if n.parent is None :
      s.root = None
    elif n.parent.left == n :
      n.parent.left = None
    else :
      n.parent.right = None

    del(s.lkp[n.key])

  def removeWithOneChild( s, n ) :
    child = n.left if n.left is not None else n.right
    if n.parent is None :
      s.root = child
      child.parent = None
    elif n.parent.left == n :
      n.parent.left = child
      child.parent = n.parent
    else :
      n.parent.right = child
      child.parent = n.parent

    del( s.lkp[n.key] ) 

  def removeWithTwoChild( s, n ) :
    suc = n.right
    while( suc.left is not None ) :
      suc = suc.left

    del(s.lkp[n.key] ) 

    n.key = suc.key
    n.pri = suc.pri
    n.seg = suc.seg
    s.remove(suc)
    s.lkp[n.key] = n
    
  def remove( s, n ) :
    if n is None :
      return
    elif n.left is None and n.right is None :
      s.removeLeaf( n )   
    elif n.left is None or n.right is None :
      s.removeWithOneChild( n ) 
    else :
      s.removeWithTwoChild( n )

  def next( s, key ): 
    n = s.getByKey( key ) 
    return s._next( n )
  def _next( s, n ):
    if n == None : 
      return None
    elif n.right is not None :
      suc = n.right
      while( suc.left is not None ) : 
        suc = suc.left
      return suc.seg
    else :
      if n.parent is not None :
        if n.parent.left == n :
          return n.parent.seg
        else:
          n.parent.right = None
          val = s._next( n.parent )
          n.parent.right = n
          return val

  def prev( s, key): 
    n = s.getByKey( key ) 
    return s._prev( n )
  def _prev( s, n ):
    if n == None : 
      return None
    elif n.left is not None :
      suc = n.left
      while( suc.right is not None ) : 
        suc = suc.right
      return suc.seg
    else :
      if n.parent is not None :
        if n.parent.right == n :
          return n.parent.seg
        else:
          n.parent.left = None
          val = s._prev( n.parent )
          n.parent.left = n
          return val

class Heap(object):
  def __init__(s) : s.data = []; 
  def push(s,e)   : heappush(s.data, e)
  def pop(s)      : return heappop(s.data) 
  def __len__(s)  : return len(s.data)
  def __iter__(s) : return s
  def remove(s,k) : 
    for i,e in enumerate( s.data ) : 
      if(  e.x == k.x and e.y == k.y ) :
        s.data[i] = s.data[-1] 
        s.data.pop()
        heapify( s.data )
        return 

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
        print( "%3.2f %3.2f"%(round(a.x,2),round(a.y,2))) 
        sys.exit(1)

    if below : 
      a = this.intersect( below ) 
      if a:
        print( "%3.2f %3.2f"%(round(a.x,2),round(a.y,2))) 
        sys.exit(1)
    
  if( event.type == Type.R ) : 

    this  = event.seg1
    above = tree.next( this.l ) 
    below = tree.prev( this.l )
    tree.deleteByKey( this.l ) 

    if above and below :
      a = above.intersect(below) 
      if a : 
        print( "%3.2f %3.2f"%(round(a.x,2),round(a.y,2))) 
        sys.exit(1)

    

        heap.push(Event( a.x, a.y, Type.I, below, this ) ) 
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

    

