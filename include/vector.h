#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct Vector2
{
    float data[2];
};

struct Vector3
{
    float data[3];
};

void initVector(float *vectorData, const float *source, size_t size);

#endif