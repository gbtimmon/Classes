from Point import Point
class Segment ( object ) :

  "A line segment"
  "============================"
  "   __eq__         : are these the same segment"
  "   slope          : get slope"
  "   get_point_at_y : finds a projection point on the y line"
  "   intersect      : do we interect? If so where"

  def __init__( self, s, e ) : 
    self.l     = s
    self.r     = e

  def slope(s):
    if s.l.x == s.r.x : 
      return None
    else:
      return float(s.r.y-s.l.y)/(s.r.x-s.l.x)

  def __eq__( self, other ) :
    "Checks if two segments are the same, even if the order of points is not"
    "ie. Segment( a, b ) == Segment(b, a )  will return True. "
    return (
          ( self.left == other.left and self.right == other.right ) 
       or ( self.left == other.right and self.right == other.left )
    )

  def __repr__(self) : return self.__str__()
  def __str__( self ) : 
    return "%s<->%s"%(self.l, self.r ) 

  def intersect( s, o ) :

    p1 = s.l
    p2 = s.r
    p3 = o.l
    p4 = o.r

    v2 = p2 - p1 
    v4 = p4 - p3


    num = (p1 - p3).dot( v2.perp() )
    den = v4.dot( v2.perp() ) + Point.eps

    t4  = float( num ) / float( den )

    num = (p3 - p1).dot( v4.perp() )
    den = v2.dot( v4.perp() ) + Point.eps

    t2  = float( num ) / float( den )    

    I = p3 + ( v4.scale( t4 ) ) 
    print( "SUGG", s, o, I ) 
    if( not( 0.0 <= t2 <= 1.0 and 0.0 <= t4 <= 1.0 ) ) : 
      return None 

    return I 


