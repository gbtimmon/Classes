from __future__ import print_function
import sys

class Node :
  def __init__( self, name ) : 
    self.name     = name 
    self.nout      = []
    self.nin       = []
    self.visited  = False
    self.comp     = None

  def __str__( self ) : 
    return (
            "%s"%(self.name) 
           + "\n    IN  : %s"%(str([x.name for x in self.nin]))
           + "\n    OUT : %s" %(str([x.name for x in self.nout]))
           + "\n    CMP : %s"%(self.comp.name)
    )

class Graph : 
  def __init__( self, arr ) : 
    self.nodes  = []
    self.lookup = {} 
    self.L = []

    state = 0
    for x in arr :
      if   state == 0 and x[0].isdigit() : 
        state = 1
      elif state == 1 and x[0].isdigit() : 
        state = 2
      elif state == 1 : 
        self.lookup[x[0]] = len( self.nodes ) 
        self.nodes.append(Node(x[0]))
      elif state == 2 :
        frm  = self.getNode(x[0])
        to   = self.getNode(x[1])
        frm.nout.append( to ) 
        to.nin.append( frm ) 

    for x in self.nodes :
      self.visit(x)

    for x in reversed( self.L ) :
      self.assign( x, x )
   
  def assign( self, x, y ) : 
    if x.comp is None :
      x.comp = y
      for n in x.nin : self.assign( n, y )

  def visit(self,  x ) :
    if not x.visited :
      x.visited = True
      for y in x.nout : self.visit( y ) 
      self.L.append( x )

  def getNode( self, name ) : 
    return self.nodes[ self.lookup[ name ] ] 

  def SCC( self ) : 

    seen = set()
    for x in self.nodes :

      if x.comp.name in seen : 
        continue 
      comp = []
      for y in self.nodes :
        if y.comp.name == x.comp.name :
          comp.append( y.name ) 

      seen.add( x.comp.name )
      if len(comp) > 1 :
        print( *comp, sep=" ")
         



  def __str__( self ) : 
    ss = "Graph : \n" 
    ss += "\n".join([str(x) for x in self.nodes])
    return ss
   


f1 = [ x.split() for x in map(str.strip, sys.stdin.readlines() ) ]  
g = Graph( f1 )
g.SCC()

