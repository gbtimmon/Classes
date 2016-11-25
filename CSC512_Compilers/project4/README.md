There is no difference between my gtx480 and gtx780 code. I wasnt able to find anything the significantly improved on but not the other.

Files

project4
  + gtx480
  |   + makefile
  |   + norm.cu   # original kernal
  |   + opt1.cu   # loop unrolling and operator reduction
  |   + opt2.cu   # Branching reduction
  |   + opt3.cu   # Increased paralellism
  |   + optAll.cu # All optimizations
  |
  + gtx780
  |   + makefile
  |   + norm.cu   # original kernal
  |   + opt1.cu   # loop unrolling and operator reduction
  |   + opt2.cu   # Branching reduction
  |   + opt3.cu   # Increased paralellism
  |   + optAll.cu # All optimizations
  |
  + README.md     # this file
  + Report.pdf    # Writeup in results

