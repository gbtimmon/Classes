#ifndef BUILTINS_H
#define BUILTINS_H

#include <parse.h>

typedef int(*Builtin)(Cmd);
int isBuiltin(const char *);
Builtin getBuiltin(Cmd); 
#endif
