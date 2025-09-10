#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"

typedef struct
{
    Vector3 *vertices;
    size_t vertexCount;
    size_t (*faces)[6];
    size_t faceCount;
} Model;

Model *initModel();
int loadModelFromFile(Model *model, const char *path);
void freeModel(Model *model);

#endif