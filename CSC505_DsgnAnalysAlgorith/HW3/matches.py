import sys

f1 = open(sys.argv[1], "r") 
f2 = open(sys.argv[2], "r")

a = map(str.strip, f1.readlines() )
b = map(str.strip, f2.readlines() )

good = 0 
for x,y in zip(a,b) :
  stat = "OK" if x == y else "NO"
  if x != y : good += 1
  print ( stat + "\n a > " + str(x) + "\n b > " + str(y) )

print( good == 0 )

