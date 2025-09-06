#ifndef MODEL_H
#define MODEL_H

#include "vector.h"

typedef struct
{
    size_t vertexCount;
    Vector3 *vertices;
    size_t faceCount;
    size_t (*faces)[3];
} Model;

int initModel(Model *model);
int loadModelFromFile(Model *model, const char *path);
int freeModel(Model *model);

#endif