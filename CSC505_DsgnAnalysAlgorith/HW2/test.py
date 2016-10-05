from __future__ import division
from sys        import argv
from random     import randint, random
from heap3      import Heap3
from heap2      import Heap2
from timer      import function_data

import matplotlib.pyplot as plt

PERC = .80
tst = int(argv[1])
rng = int(argv[2])


print "Starting test"
for i in xrange( tst ) : 

  print "Running Test : " , i 
  heaps = [ Heap2() , Heap3() ]
  for j in xrange( rng ) : 
    random1 = randint(0,1000)
    random2 = randint(0,1000)
    heaps[0].insert(random1) 
    heaps[1].insert(random1) 
    heaps[0].remove()     
    heaps[1].remove()      
    heaps[0].insert(random2)
    heaps[1].insert(random2) 

  

def unwrap ( v ) :
  return [ sum(v[x]) / len(v[x]) if x in v else 0 for x in range(max(v.keys())+1) ]
   
h2i = function_data['heap2.Heap2']['insert']
h2r = function_data['heap2.Heap2']['remove']
h3i = function_data['heap3.Heap3']['insert']
h3r = function_data['heap3.Heap3']['remove']

fig = plt.figure()
ax1 = fig.add_subplot(211)
ax1.scatter( range( len( h2r) )[::25], unwrap( h2r )[::25], color='r', marker='x', label='Heap2')
ax1.scatter( range( len( h3r) )[::25], unwrap( h3r )[::25], color='b', marker='x', label='Heap3')
ax2 = fig.add_subplot(212)
ax2.scatter( range( len( h2i) + 1)[::25], unwrap( h2i )[::25], color='r', marker='x', label='Heap2')
ax2.scatter( range( len( h3i) + 1)[::25], unwrap( h3i )[::25], color='b', marker='x', label='Heap3')

plt.show()

      
    
    
  
  

