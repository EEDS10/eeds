#include <stdio.h>
#include <sys/time.h>
#include "utils.h"

struct timeval tv;

long gettime(){

    gettimeofday(&tv, NULL);
    
    long output = tv.tv_sec * 1000;
    output += tv.tv_usec  / 1000;

    return output;
}
