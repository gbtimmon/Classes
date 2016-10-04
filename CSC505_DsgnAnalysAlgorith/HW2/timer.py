import sys
import time 
import numpy

def getTime ():
  return time.time()

function_data = dict()
"""
Decorator to time a function
"""

def timeFunction( func ) : 
  function_data[ func ] = []
  
  def wrapper (*args, **kwargs ) :
    t0  = getTime() 
    ret = func(*args, **kwargs)
    t1  = getTime()

    function_data[ func ].append( t1 - t0 )

    return ret

  return wrapper 


def report() :
  for k, v in function_data.iteritems() : 
    print k.__name__, v
