#ifndef TYPES_H
#define TYPES_H

typedef struct
{
    unsigned char r, g, b;
} Color;

unsigned char toGray(Color color);

#endif