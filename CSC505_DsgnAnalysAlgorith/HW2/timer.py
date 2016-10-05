import sys
import time 
from collections import defaultdict as ddict


#               Class(          Func  (         Count  ( time ) ) )
function_data = ddict( lambda : ddict ( lambda : ddict ( list ) ) ) 


def getTime ():
  return int( round ( time.clock() * 1000 * 1000 * 1000) )

"""
Decorator to time a function
"""
def timeFunction( func ) : 
  
  def wrapper (*args, **kwargs ) :
    t0  = getTime() 
    ret = func(*args, **kwargs)
    t1  = getTime()

    function_data[ str(args[0].__class__) ][ str(func.__name__) ][ len(args[0]) ].append( t1 - t0 )
    return ret

  return wrapper 

def report() :
  print "%20s | %6s | %13s | %13s" % ("FUNCTION", "CALLS", "AVG", "STD DEV")
  for k, v in function_data.iteritems() : 
    #print "%20s | %6d | %13.10f | %13.10f" % (k.__name__, len(v), mean(v), pstdev(v) )
    for k, v, in v.iteritems() :
      print k, v
