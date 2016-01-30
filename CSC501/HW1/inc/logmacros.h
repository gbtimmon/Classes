#ifndef LOGMARCO_H
#define LOGMACRO_H

    #define INFO 
    #define MYTRHEAD_C_STACK
    #define THREAD_C_STACK
  
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_RESET   "\x1b[0m"

    #define COLOR_I ANSI_COLOR_CYAN
    #define C_MT ANSI_COLOR_GREEN
    #define C_TCS ANSI_COLOR_YELLOW
    #define C_OFF ANSI_COLOR_RESET

    #ifdef MYTRHEAD_C_STACK 
        #define mcs(x)    printf(C_MT "[DEBUG] %s\n"      C_OFF, x);
        #define mcsf(x,y) printf(C_MT "[DEBUG] %s : %d\n" C_OFF, x,y);
    #else
        #define mcs(x)
        #define mcsf(x,y);
    #endif
    
    #ifdef INFO
        #define info(x)    printf(COLOR_I "[INFO] %s\n"      C_OFF, x);
        #define infof(x,y) printf(COLOR_I "[INFO] %s : %d\n" C_OFF, x,y);
    #else
        #define info(x)
        #define infof(x,y)
    #endif
    #ifdef THREAD_C_STACK 
        #define tcs(x)   printf(C_TCS "[INFO] %s\n"      C_OFF, x);
        #define tcsf(x,y) printf(C_TCS "[INFO] %s : %d\n" C_OFF, x,y);
    #else
        #define tcs(x)
        #define tcsf(x,y)
    #endif

#endif
