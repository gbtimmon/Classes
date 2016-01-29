#ifndef LOGMARCO_H
#define LOGMACRO_H

    #define INFO 
    #define DEBUG

    #ifdef DEBUG 
        #define debug(x) printf("[DEBUG] %s\n",x);
        #define debugf(x,y) printf("[DEBUG] %s : %d\n",x,y);
    #else
        #define debug(x)
        #define debugf(x,y);
    #endif
    
    #ifdef INFO 
        #define info(x)  printf("[INFO] %s\n",x);
        #define infof(x,y) printf("[INFO] %s : %d\n",x,y);
    #else
        #define info(x)
        #define infof(x,y)
    #endif

#endif
