f1 <- function( x ) (2 * (x^2)) + (5 * x * log2( x ))
g1 <- function( x ) (10 * (x^ 2 ))
r1 <- function( x ) ( f1(x) - g1(x) )


png(filename="plot1.png")
plot  ( .1:100, f1(.1:100), pch=21, col="red" )
points( .1:100, g1(.1:100), pch=21, col="blue" )

root1 = uniroot( r1, c(.1,100))$root
abline( v = root1 )
print( root1 )


f2 <- function( x ) 3 * x * log2( x )
g2 <- function( x ) 2 * (x ^ 1.5 )
r2 <- function( x ) ( f2(x) - g2(x) ) 

png(filename="plot2.png")
plot  ( 80:180, f2(  80:180), pch=21, col="red" )
points( 80:180, g2(  80:180), pch=21, col="blue" )

root2 = uniroot( r2, c(80,180))$root
abline( v = root2 )
print( root2 )



