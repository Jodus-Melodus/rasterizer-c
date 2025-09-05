#include <string.h>
#include "vector.h"

void initVector(float *vectorData, const float *source, size_t size)
{
    if (vectorData == NULL || source == NULL)
        return;

    memcpy(vectorData, source, size * sizeof(float));
}

float getVector(float *vectorData, size_t size, size_t index)
{
    if (vectorData == NULL || index >= size)
        return 0.0f;
    return vectorData[index];
}
