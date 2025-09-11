#ifndef TYPES_H
#define TYPES_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

typedef struct
{
    unsigned char r, g, b, a;
} PixelColor;

char asChar(const PixelColor color);

typedef enum
{
    X,
    Y,
    Z
} Axis;

typedef struct
{
    float x, y;
} Point2D;

typedef struct
{
    float x, y, z;
} Point3D;

#endif

// TODO add error enum type