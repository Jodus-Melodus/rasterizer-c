#ifndef MODEL_H
#define MODEL_H

#include "vector.h"

struct Model
{
    size_t vertexCount;
    struct Vector3 *vertices;
    size_t faceCount;
    size_t (*faces)[3];
};

int initModel(struct Model *model);
int loadModelFromFile(struct Model *model, const char *path);
int freeModel(struct Model *model);

#endif