#ifndef BUILTINS_C
#define BUILTINS_C

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <builtins.h>
#include <unistd.h>

#define OK     0
#define NOT_OK 1

#define STR_EQ(x,y)            !strcmp(x,y)
#define B_BG                   "bg"
#define B_CD                   "cd"
#define B_ECHO                 "echo"
#define B_FG                   "fg"
#define B_JOBS                 "jobs"
#define B_KILL                 "kill"
#define B_LOGOUT               "logout"
#define B_NICE                 "nice"
#define B_PWD                  "pwd"
#define B_SETENV               "setenv"
#define B_UNSETENV             "unsetenv"
#define B_WHERE                "where"

int f_BG (Cmd c){
    dprintf(STDERR_FILENO, "BG: not yet implemented");
    return OK;
}

int f_CD (Cmd c){
    const char * cd = c->args[1];
    if( chdir(cd) ) return NOT_OK;
    return OK;
}

int f_ECHO (Cmd c){
    for ( int i = 1; c->args[i] != NULL; i++ )
        printf("%s ", c->args[i]);
    printf("\n"); 
    return OK;
}

int f_FG (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_JOBS (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_KILL (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_LOGOUT (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_NICE (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}


int f_PWD (Cmd c){
    char * pwd = getenv("PWD");
    if( !pwd ) {
        dprintf(STDERR_FILENO, "Pwd: Something terrible has happened!\n");
        return NOT_OK;
    }  
    dprintf(STDOUT_FILENO, "%s", pwd);
    return OK;
}

int f_SETENV (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_UNSETENV (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

int f_WHERE (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}

Builtin getBuiltin(Cmd c){

    const char* arg = c->args[0]; 

    if (STR_EQ(arg,B_BG) )            { return *f_BG; }
    else if (STR_EQ(arg,B_CD) )       { return *f_CD; }
    else if (STR_EQ(arg,B_ECHO) )     { return *f_ECHO; }
    else if (STR_EQ(arg,B_FG) )       { return *f_FG; }
    else if (STR_EQ(arg,B_JOBS) )     { return *f_JOBS; }
    else if (STR_EQ(arg,B_KILL) )     { return *f_KILL; }
    else if (STR_EQ(arg,B_LOGOUT) )   { return *f_LOGOUT; }
    else if (STR_EQ(arg,B_NICE) )     { return *f_NICE; }
    else if (STR_EQ(arg,B_PWD) )      { return *f_PWD; }
    else if (STR_EQ(arg,B_SETENV) )   { return *f_SETENV; }
    else if (STR_EQ(arg,B_UNSETENV) ) { return *f_UNSETENV; }
    else if (STR_EQ(arg,B_WHERE) )    { return *f_WHERE; }
    else                              { return NULL; }
}


#endif
