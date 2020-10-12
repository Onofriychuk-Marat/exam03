#ifndef HEADER_H
#define HEADER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

typedef struct  square_s
{
    int         type;
    float       x;
    float       y;
    float       size_x;
    float       size_y;
    int         size_map_x;
    int         size_map_y;
    char        symbol;
}               square_t;

#endif