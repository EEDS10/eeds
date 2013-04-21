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


int strbeginswith(char* str, char *beginswith){
    int i = 0;
    while(str[i] == beginswith[i]){
        if(str[i+1] == '\0' || beginswith[i+1] == '\0'){
            return 1;
        }
        i++;
    }
    return 0;
}
