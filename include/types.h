#ifndef TYPES_H
#define TYPES_H

struct Color
{
    unsigned char r, g, b;
};

unsigned char toGray(struct Color color);

#endif