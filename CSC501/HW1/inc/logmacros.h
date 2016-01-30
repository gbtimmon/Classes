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

    #define C_INF ANSI_COLOR_CYAN
    #define C_MCS ANSI_COLOR_GREEN
    #define C_TCS ANSI_COLOR_YELLOW
    #define C_OFF ANSI_COLOR_RESET

    #define puts(args...)    printf("[STDOUT  ]     ");      printf(args); printf("\n" C_OFF)

    #ifdef MYTRHEAD_C_STACK 
        #define mcs(args...) printf(C_MCS "[MYTHREAD] "); printf(args); printf("\n" C_OFF)
    #else
        #define mcs(args...)
    #endif
    
    #ifdef INFO
        #define info(args...) printf(C_INF "[INFO    ]     "); printf(args); printf("\n" C_OFF)
    #else
        #define info(args...)
    #endif

    #ifdef THREAD_C_STACK 
        #define tcs(args...) printf(C_TCS "[THREAD  ]   "); printf(args); printf("\n" C_OFF)
    #else
        #define tcs(x)
    #endif

    

#endif
