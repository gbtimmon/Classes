from __future__ 	import print_function
from collections 	import defaultdict
import math 

class GiniNode() : 
  
    def __init__(self, data, keyCol, maxDepth ) :

        self._keyCol   = keyCol 
        self._data     = data
        self._maxDepth = maxDepth
        self._depth    = 0
        self._impurity = impurity( getCol(self._data, keyCol))

    def __repr__ (self) : 
        return ( "(  _keyCol = {0}, _impurity = {1} data ={2})".format( self._keyCol, self._impurity, self._data) )

def impurity(data) :
    its = defaultdict(float)
    for d in data : 
        its[d] += 1.0
    denom = float(sum(its.values()))
    imp = 1.0
    for v in its.values() : 
        imp += - ((v / denom)**2)
    return imp

def getCol(data, ind) :
    col = []
    for d in data : 
        col.append(d[ind])
    return col

def splitMat(data, col) : 
    left = []
    cent = []
    rght = []
    for d in data : 
        left.append(d[:col])
        cent.append(d[col])
        rght.append(d[col+1:])
    return (left, cent, rght )

def readInput( filename ) : 
    data = []
    for line in open(filename) : 
        data.append( line.strip().split(','))
    for d in data : 
        del( d[0] )
    return data
     
if __name__ == '__main__' :  
    x = GiniNode( readInput( 'data/data.txt'), 4, 1)
    print( x )  
   

