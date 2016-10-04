import sys
import time 

def getTime ():
  return time.time()

function_data = dict()
"""
Decorator to time a function
"""

def timeFunction( func ) : 
  function_data[ func ] = ([], [])
  
  def wrapper (*args, **kwargs ) :
    t0  = getTime() 
    ret = func(*args, **kwargs)
    t1  = getTime()

    function_data[ func ][1].append( t1 - t0 )
    function_data[ func ][1].append( len(args[0]) )

    return ret

  return wrapper 

def mean(data):
    """Return the sample arithmetic mean of data."""
    n = len(data)
    if n < 1:
        raise ValueError('mean requires at least one data point')
    return sum(data)/n # in Python 2 use sum(data)/float(n)

def _ss(data):
    """Return sum of square deviations of sequence data."""
    c = mean(data)
    ss = sum((x-c)**2 for x in data)
    return ss

def pstdev(data):
    """Calculates the population standard deviation."""
    n = len(data)
    if n < 2:
        raise ValueError('variance requires at least two data points')
    ss = _ss(data)
    pvar = ss/n # the population variance
    return pvar**0.5

def report() :
  print "%20s | %6s | %13s | %13s" % ("FUNCTION", "CALLS", "AVG", "STD DEV")
  for k, v in function_data.iteritems() : 
    #print "%20s | %6d | %13.10f | %13.10f" % (k.__name__, len(v), mean(v), pstdev(v) )
    print k , v
