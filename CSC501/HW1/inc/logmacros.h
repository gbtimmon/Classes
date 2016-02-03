#ifndef LOGMARCO_H
#define LOGMACRO_H

  
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_RESET   "\x1b[0m"

    #define THREAD_C_STACK
    #define INFO

    #define C_INF ANSI_COLOR_CYAN
    #define C_MCS ANSI_COLOR_GREEN
    #define C_TCS ANSI_COLOR_YELLOW
    #define C_OFF ANSI_COLOR_RESET

    #define puts(args...)    printf("[STDOUT  ]     ");      printf(args); printf("\n" C_OFF)

    #ifdef INFO
        #define info(args...) printf(C_INF "[INFO    ]     "); printf(args); printf("\n" C_OFF)
    #else
        #define info(args...)
    #endif

    #ifdef THREAD_C_STACK 
        #define tcs(args...) printf(C_TCS "[THREAD%2.0d]   ", (q_running != NULL) ? q_running->id :-1); printf(args); printf("\n" C_OFF)
    #else
        #define tcs(args...) 
    #endif

#endif
