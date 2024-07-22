
#include <time.h>

#include "system.h"


#define LIB_SYSTEM_ITERATION_LIMIT_SMALL 8

void delay_ms( unsigned int time_ms ) {
    struct timespec req, remain;
    int it = 0;

    req.tv_sec  = (time_t)(time_ms / 1000);
    req.tv_nsec = (long)(time_ms % 1000) * 1000000 ;

    while( nanosleep( &req, &remain ) != 0 ){
        req.tv_sec = remain.tv_sec;
        req.tv_nsec = remain.tv_nsec;

        if( (it++) >= LIB_SYSTEM_ITERATION_LIMIT_SMALL ) {
            break;
        }
    }
}

