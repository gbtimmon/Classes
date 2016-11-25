class Point( object ) : 
  "Point class to save a point and define some basic"
  "equality functions"

  eps = 0.000000001
 
  def __init__( self, x, y ) : 
    self.x = float(x)
    self.y = float(y)

  def __repr__( s ) : return s.__str__()
  def __str__( s ) : 
    xx = round( s.x, 2 )
    yy = round( s.y, 2 ) 
    return "(%4.2f, %4.2f)"%(xx,yy)

  def __eq__( s, o ) :
    return Point.y_sorter( s, o ) == 0

  def __lt__( s, o ) :
    return Point.y_sorter( s, o ) == -1

  def __gt__( s, o ) : 
    return Point.y_sorter( s, o ) == 1 

  def __ge__( s, o ) :
    return Point.y_sorter( s, o ) >= 0

  def __le__( s, o ) : 
    return Point.y_sorter( s, o ) <= 0
 
  def __sub__( s, o ) : 
    return Point( s.x - o.x, s.y - o.y )

  def __add__( s, o ) : 
    return Point( s.x + o.x, s.y + o.y )

  def scale( s, m ) :
    return Point( s.x * m, s.y * m )

  def perp( s ) :
    return Point( -s.y, s.x )

  def dot( s, o ) :
    return ( s.x * o.x ) + (  s.y * o.y )

  @staticmethod
  def cross( s, o ) : 
    return (s.x * o.y) - ( o.x * s.y )

  @staticmethod
  def eq( a, b ) : 
    return a == b

  @staticmethod 
  def x_eq( a, b ) : 
    return abs( a.x - b.x ) < Point.eps 

  @staticmethod 
  def y_eq( a, b ) : 
    return abs( a.y - b.y ) < Point.eps 
  
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
