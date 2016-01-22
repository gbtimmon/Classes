#!/usr/bin/python
####################
# GDE calculator
####################
import sys

def xsplit(line) :
  a, b = line.split() 
  return( int(a), int(b) )


def main() : 
  print "filename 1 : ", sys.argv[1]
  print "filename 2 : ", sys.argv[2]
  
  with open(sys.argv[1]) as fp : 
    Vi, Ei = xsplit(fp.readline())

  with open(sys.argv[2]) as fp : 
    _Vj, Ej = xsplit(fp.readline())
    ELj = [ [] for i in xrange( _Vj ) ]

    for line in fp : 
      e1, e2 = xsplit(line)
      if not e2 in ELj[e1]:
        ELj[e1].append(e2)
      if not e1 in ELj[e2]:
        ELj[e2].append(e1)

  
  Vi = 0
  Vj = 0 
  Vij = 0
  Eij = 0 
  for i in xrange(min(_Vi, _Vj)) : 
    print ELi[i]
    if ELi[i] :
      print "is not empty : ", i
      Vi += 1
    if ELj[i] :
      Vj += 1   
    if ELi[i] and ELj[i] : 
      Vij += 1
      Eij += len ( [ k for k in Vi[i] if k in Vj[i] ] )
  
  print "Vi ", Vi
  print "Ei ", Ei
  print "Vj ", Vj
  print "Ej ", Ej
  print "Vij ", Vij
  print "Eij ", Eij

if __name__ == '__main__':
  main()


