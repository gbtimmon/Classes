#!/usr/bin/python
####################
# GDE calculator
####################
import sys
import glob
import itertools
import os 

L = 5 #window Size

import re

def tryint(s):
    try:
        return int(s)
    except:
        return s
     
def alphanum_key(s):
    """ Turn a string into a list of string and number chunks.
        "z23a" -> ["z", 23, "a"]
    """
    return [ tryint(c) for c in re.split('([0-9]+)', s) ]

def sort_nicely(l):
    """ Sort the given list in the way that humans expect.
    """
    l.sort(key=alphanum_key)

def pairs(iterable):
    a, b = itertools.tee(iterable)
    next(b, None)
    return itertools.izip(a, b)

class Graph : 
  def __init__ (self, filename) : 

    print "Building Graph : ", filename

    self.nodeSet = set()
    self.edgeSet = set()
    
    with open(filename) as fp :
      fp.readline() #skip first line 
      for line in fp : 
        v1 , v2 = line.split()
        self.addEdge(v1,v2)

  # In our implementation, 
  # as is imply by the paper, all edges are assume to have
  # weight 1, and are onyl measure on existence in the GED 
  # the MGED implementations. 
  def addEdge(self, v1, v2) : 
    self.nodeSet.add(v1)
    self.nodeSet.add(v2)
    self.edgeSet.add(str(v1) + "->" + str(v2))

  def editDistance(self, other):
    Vg   = len(self.nodeSet)
    Vh   = len(other.nodeSet)
    VgVh = len(self.nodeSet & other.nodeSet)
    Eg   = len(self.edgeSet)
    Eh   = len(other.edgeSet)
    EgEh = len(self.edgeSet & other.edgeSet)
    return (Vg + Vh - 2*VgVh + Eg + Eh - 2*EgEh)
     
def computeThreshold (dists) : 
  median = sorted(dists)[len(dists)/2]
  mean   = sum(dists) / len(dists)
  summed = 0 
  for i in dists : 
    summed += (i - mean)**2
  stddev  = (summed / len(dists))**.5
  print median, stddev
  return (median - (2*stddev)) , (median + (2*stddev))
  
def findMedianGraph ( gph ) :
  distSums = []
  for gi in gph : 
    sum = 0 
    for gj in gph : 
      sum += gi.editDistance(gj)
    distSums.append(sum)

  return gph[distSums.index(min(distSums))]

def main() : 
  print "Algoritm    : ", sys.argv[1]
  print "Filepattern : ", sys.argv[2]
  print "Output      : ", sys.argv[3]
  files = glob.glob(sys.argv[2])
  sort_nicely(files)
  print "Files       : ", files
  graphs = [ Graph(filename) for filename in files ] 

  dists = []

  if sys.argv[3] == '-' : 
    output = sys.stdout
  else : 
    if os.path.isfile(sys.argv[3]):
      print "Warning,", sys.argv[3], " already exists. Exiting. "
      exit(1) 
    output = open(sys.argv[3], 'w')

  if sys.argv[1] == 'GED' : 

    for i,j in pairs(graphs) : 
      dists.append(i.editDistance(j))
 
    lower_bound, upper_bound = computeThreshold(dists)
    print "---OUTPUT---"
    sys.stdout = output
    print "{0}\t{1}".format(lower_bound, upper_bound)
    print "{0}\t{1}".format(0 , "NAN")
    for idx, val in enumerate(dists) :
      print "{0}\t{1}".format(idx + 1, val)

  elif sys.argv[1] == 'MGED' :
    for x in range(L, len(graphs)) : 
      print x, "[ ",(x-L), ":", (x-1), "]"
      medianGraph = findMedianGraph( graphs[(x-L):(x-1)] )
      dists.append(graphs[x].editDistance(medianGraph))

    lower_bound, upper_bound = computeThreshold(dists)
    print "---OUTPUT---"
    sys.stdout = output
    print "{0}\t{1}".format(lower_bound, upper_bound)
    for x in xrange(L) : 
      print "{0}\t{1}".format(x, "NAN")
    for idx, val in enumerate(dists) : 
      print "{0}\t{1}".format(idx + L, val)
    
  else : 
    exit (1)



    

  


if __name__ == '__main__':
  main()


