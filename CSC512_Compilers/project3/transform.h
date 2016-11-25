#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "token.h"
/** 
  Transform is responsible for reforming the original tree into a tree in a shape good for output. 
  The transformations occur in a Depth first search manner. This ensures :

  1.) That all nodes below the current node are transformed before the current node, making reasoning
  about transforms simple. 

  2.) That I can make in place changes to the tree without worrying about messing up the order of 
  transofmration. For instance I can switch the order of one sibling with another without changing 
  the order of transformations since the order of transformations is already encoded into the stack by the recursion.
 
  Transform is the main function here -- it calls the DFS. then each node is process with a transform function that corresponds to the node type. 

  Much of the 'attribute' information is built into the tree rather than as value in the token.
  For example : Type of a var is implicit based on the ordering of tokens in the data element. 
  Maybe not the best was to do things, but good enough for quick work. 

  Some helper function were added to the token library to assist in transformations

  At the end of the transform, the SymbolTables are built.
**/

void transform( Token ); 
// The only public function. Transforms a tree inplace to the Correct form. 
// @1   : root of the tree. 

#endif
