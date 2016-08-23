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

#define YES    1
#define NO     0

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

extern int STDOUT_FD;
extern int STDERR_FD; 
extern int STDIN_FD;

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
    exit(0); 
    return OK;
}

int f_NICE (Cmd c){
    dprintf(STDERR_FILENO, "FG: not yet implemented");
    return OK;
}


int f_PWD (Cmd c){
    dprintf(STDOUT_FILENO, "%s\n", getcwd(NULL, 0));
    return OK;
}

int f_SETENV (Cmd c){

    if( c->nargs == 1 ) {

        extern char** environ;
        for(int i = 0; environ[i]; i++){
            dprintf(STDOUT_FD, "%s\n", environ[i]);
        }

    } else if( c->nargs == 2 ) { 

       setenv( c->args[1], "", 1);
    
    } else {

       setenv( c->args[1], c->args[2], 1);

    } 
    return OK;
}

int f_UNSETENV (Cmd c){
    if(c->nargs == 1 ) 
        return NOT_OK;
    else 
        unsetenv(c->args[1]);
    return OK;
}

int f_WHERE (Cmd c){
    if ( isBuiltin(c->args[1]) ) 
        dprintf(STDOUT_FD, "%s is a builtin\n", c->args[1]);
       
    char buf[1024];
    char* src = strdup( getenv( "PATH") );
    char delims[] = ":";
    char* ptr = strsep( &src, delims);

    while( ptr ) {
        strcpy(buf,ptr);
        strcat(buf,"/");
        strcat(buf,c->args[1]);
        if( access(buf, X_OK ) != -1)
            printf("%s\n", buf);
        ptr = strsep( &src, ":" );
         
    }
    free(src);
    return OK;
} 

int isBuiltin(const char * arg){
    if (STR_EQ(arg,B_BG) )            { return YES;}
    else if (STR_EQ(arg,B_CD) )       { return YES;}
    else if (STR_EQ(arg,B_ECHO) )     { return YES;}
    else if (STR_EQ(arg,B_FG) )       { return YES;}
    else if (STR_EQ(arg,B_JOBS) )     { return YES;}
    else if (STR_EQ(arg,B_KILL) )     { return YES;}
    else if (STR_EQ(arg,B_LOGOUT) )   { return YES;}
    else if (STR_EQ(arg,B_NICE) )     { return YES;}
    else if (STR_EQ(arg,B_PWD) )      { return YES;}
    else if (STR_EQ(arg,B_SETENV) )   { return YES;}
    else if (STR_EQ(arg,B_UNSETENV) ) { return YES;}
    else if (STR_EQ(arg,B_WHERE) )    { return YES;}
    else                              { return NO;}
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
