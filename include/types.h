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
} Matrix4;

Matrix4 perspectiveMatrix4(float fov, float aspect, float near, float far);
Vector4 TransformVector4(Matrix4 m, Vector4 v);
#endif

// TODO add error enum type