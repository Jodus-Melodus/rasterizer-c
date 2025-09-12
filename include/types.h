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

typedef struct
{
    float x, y, z, w;
} RasterVector4;

typedef struct
{
    float m[4][4];
} RasterMatrix4;

RasterMatrix4 perspective(float fov, float aspect, float near, float far);
RasterVector4 mat4_mul_vec4(RasterMatrix4 m, RasterVector4 v);
#endif

// TODO add error enum type