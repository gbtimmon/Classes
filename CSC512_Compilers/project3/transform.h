#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "token.h"


struct xData {
    int varCount; 
};

char * getPayloadString( Token );
void transform( Token ); 

#endif
