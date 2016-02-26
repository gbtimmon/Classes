#ifndef BUILTINS_C
#define BUILTINS_C


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <builtins.h>

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
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_CD (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_ECHO (Cmd c){
    for ( int i = 1; c->args[i] != NULL; i++ )
        printf("%s ", c->args[i]);
    printf("\n"); 
    return EXIT_SUCCESS;
}

int f_FG (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_JOBS (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_KILL (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_LOGOUT (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_NICE (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}


int f_PWD (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_SETENV (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_UNSETENV (Cmd c){
    fprintf(stderr, "Implementation missing\n"); 
    return EXIT_SUCCESS;
}

int f_WHERE (Cmd c){
    fprintf(stderr, "Implementation missing\n");
    return EXIT_SUCCESS;
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
