#ifndef BUILTINS_H
#define BUILTINS_H

#include <parse.h>

typedef int(*Builtin)(Cmd);
Builtin getBuiltin(Cmd); 
#endif
