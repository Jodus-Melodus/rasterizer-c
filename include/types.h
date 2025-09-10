#ifndef TYPES_H
#define TYPES_H

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

typedef struct
{
    unsigned char r, g, b;
} Color;

char asChar(const Color color);

typedef enum
{
    X,
    Y,
    Z
} Axis;

typedef struct
{
    float x, y;
} Vector2;

typedef struct
{
    float x, y, z;
} Vector3;

#endif