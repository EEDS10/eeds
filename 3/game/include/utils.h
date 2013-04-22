#ifndef UTILS_H
#define UTILS_H

#define MILLISECONDS_PER_TICK 20

#ifndef MIN
    #define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
    #define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

long gettime();
int strbeginswith(char* str, char *beginswith);

#endif
