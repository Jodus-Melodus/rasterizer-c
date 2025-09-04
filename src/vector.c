#include "vector.h"
#include <string.h>

void initVector(float *vectorData, const float *source, size_t size)
{
    if (vectorData == NULL || source == NULL)
        return;

    memcpy(vectorData, source, size);
}
