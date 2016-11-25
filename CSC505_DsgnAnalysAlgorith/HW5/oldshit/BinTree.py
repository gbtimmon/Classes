import sys
class BinTree(object) :

  class Node(object) :
      _id = 1
      def __init__( s, key, pri, seg, l, r, p ):
        s.id     = BinTree.Node._id
        s.key    = key
        s.pri    = pri
        s.seg    = seg
        s.left   = l
        s.right  = r
        s.parent = p

        BinTree.Node._id += 1

      def pri( s ) :
        m = s.seg.slope() 

      def __repr__( s ) : return s.__str__()
      def __str__( s ) :
        left  = str(s.left.id) if s.left else "None"
        par   = str(s.parent.id) if s.parent else "None"
        right = str(s.right.id) if s.right else "None"
        return "%2d\n==================\n  KEY : %10s PRI : %10s VAL : %s]\n  PAR : %10s LFT : %10s RGT : %4s]>"%(s.id, str(s.key), str(s.pri), str(s.val), par, left, right)

  def __init__(s) :
    s.root = None
    s.size = 0
    s.lkp  = {}

  def __len__(s) :
    return s.size

  def put( s, key, pri, seg ):
    s.size += 1
    if s.root == None :
      newNode    = BinTree.Node( key, pri, seg, None, None, None )
      s.root     = newNode
      s.lkp[key] = newNode
      return

    cur = s.root
    while True :
      if cur.key >= key:
        if cur.left :
          cur = cur.left
        else :
          newNode    = BinTree.Node( key, pri, seg, None, None, cur )
          cur.left   = newNode
          s.lkp[key] = newNode
          return
      else :
        if cur.right :
          cur = cur.right
        else :
          newNode    = BinTree.Node( key, pri, seg, None, None, cur )
          cur.right  = newNode
          s.lkp[key] = newNode
          return

  def getByKey( s, key ) :
    return s.lkp[key]

  def deleteByPri( s, pri ) :
    n = s.getByPri( pri )
    s.remove( n )
    s.size -= 1

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

  def printTree( s ) :

    def rec( n ) :
      if( n.left ) : rec( n.left )
      print( n )
      if( n.right ): rec( n.right )
    print("BIN TREE CONTENTS" )

    if( s.root == None ) :
      print( "empty" )
      return

    rec( s.root )
