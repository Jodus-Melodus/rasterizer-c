#ifndef TYPES_H
#define TYPES_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

typedef enum
{
    X,
    Y,
    Z
} Axis;

typedef struct
{
    float m[4][4];
} RasterMatrix4;

RasterMatrix4 perspective(float fov, float aspect, float near, float far);
Vector4 mat4_mul_vec4(RasterMatrix4 m, Vector4 v);
#endif

// TODO add error enum type