import math
from collections import defaultdict

FILENAME = 'data/data.txt'
TF_INDEX = 5

do = []
for line in open(FILENAME) : 
    do.append( line.strip().split(',') )

counts = defaultdict(int)
for arr in do : 
    counts[arr[TF_INDEX]] += 1

print counts
print sum(counts.values())

gini = 0 
for v in values() : 
    gini = math.(v,2)
    

